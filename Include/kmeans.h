#ifndef KMEANS_H
#define KMEANS_H

#include <random>
#include <iomanip>
#include "struct.h"
#include "config.h"

#define stc_cstD(x) static_cast<double>(x)
#define stc_cstI(x) static_cast<int>(x)

const double precision = 1e-9; //precision for values close to zero
int iterations = MAX_ITERARIONS;

/*functions to print the information of the clusters*/

void ClustersInfo(vector<cluster> &centroids, int knd){
    if(knd) cout<<std::setw(12)<<"Centroid"<<std::setw(10)<<"Blue(x)"<<std::setw(10)<<"Green(y)"<<std::setw(10)<<"Red(z)"<<endl;
    else cout<<std::setw(12)<<"Centroid"<<std::setw(10)<<"Blue(x)"<<std::setw(10)<<"Green(y)"<<std::setw(10)<<"Red(z)"<<std::setw(15)<<"Total pixels"<<endl;
    for(int i = 0; i < centroids.size(); i++){
        if(knd) cout<<std::setw(12)<<i<<std::setw(10)<<centroids[i].b<<std::setw(10)<<centroids[i].g<<std::setw(10)<<centroids[i].r<<endl;
        else cout<<std::setw(12)<<i<<std::setw(10)<<centroids[i].b<<std::setw(10)<<centroids[i].g<<std::setw(10)<<centroids[i].r<<std::setw(15)<<centroids[i].numPixels<<endl;
    }
}

void header(){
    cout<<std::setw(12)<<"Centroid"
    <<std::setw(10)<<"Blue(x)"
    <<std::setw(10)<<"Green(y)"
    <<std::setw(10)<<"Red(z)"
    <<std::setw(15)<<"Total blue"
    <<std::setw(15)<<"Total green"
    <<std::setw(15)<<"Total red"
    <<std::setw(15)<<"Total pixels" << endl;
}

void info(cluster &c, int &k){
    cout<<std::setw(12)<<k
    <<std::setw(10)<<c.b
    <<std::setw(10)<<c.g
    <<std::setw(10)<<c.r
    <<std::setw(15)<<c.average.b
    <<std::setw(15)<<c.average.g
    <<std::setw(15)<<c.average.r
    <<std::setw(10)<<c.numPixels << endl;
}

/*Create a summed table to calculate the sum of the pixels in a given area*/
vector<vector<averageClust>> summed_table(vector<vector<bitRGB>> &matRGB){

    vector<vector<averageClust>> ref_Img(matRGB.size(), vector<averageClust>(matRGB[0].size()));
    ref_Img[0][0] = {stc_cstD(matRGB[0][0].ble), stc_cstD(matRGB[0][0].grn), stc_cstD(matRGB[0][0].red)};

    //calculate the first row and column of the summed table
    for(int i = 1; i<matRGB.size(); i++)
        ref_Img[i][0] = {ref_Img[i-1][0].b + matRGB[i][0].ble, ref_Img[i-1][0].g + matRGB[i][0].grn, ref_Img[i-1][0].r + matRGB[i][0].red};
    for(int i = 1; i<matRGB[0].size(); i++)
        ref_Img[0][i] = {ref_Img[0][i-1].b + matRGB[0][i].ble, ref_Img[0][i-1].g + matRGB[0][i].grn, ref_Img[0][i-1].r + matRGB[0][i].red};

    //calculate the rest of the table
    for(int i = 1; i<matRGB.size(); i++)
        for(int j = 1; j<matRGB[0].size(); j++)
            ref_Img[i][j] = {ref_Img[i-1][j].b + ref_Img[i][j-1].b - ref_Img[i-1][j-1].b + matRGB[i][j].ble,
                             ref_Img[i-1][j].g + ref_Img[i][j-1].g - ref_Img[i-1][j-1].g + matRGB[i][j].grn,
                             ref_Img[i-1][j].r + ref_Img[i][j-1].r - ref_Img[i-1][j-1].r + matRGB[i][j].red};
    
    return ref_Img;
}

/*apply the box blur filter to the image to reduce the noise*/
void Box_Blur(vector<vector<bitRGB>> &matRGB, int &r){
    //Dynamic kernel
    averageClust sum;
    int area = (2*r+1)*(2*r+1);
    
    //get the summed table of the image to calculate the sum of the pixels in a given area
    vector<vector<averageClust>> ref_Img = summed_table(matRGB);

    for(int i = r + 1; i < matRGB.size() - r -1 ; i++){
        for(int j = r + 1; j < matRGB[0].size() - r - 1; j++){
            sum = {ref_Img[i+r][j+r].b - ref_Img[i-r-1][j+r].b - ref_Img[i+r][j-r-1].b + ref_Img[i-r-1][j-r-1].b,
                   ref_Img[i+r][j+r].g - ref_Img[i-r-1][j+r].g - ref_Img[i+r][j-r-1].g + ref_Img[i-r-1][j-r-1].g,
                   ref_Img[i+r][j+r].r - ref_Img[i-r-1][j+r].r - ref_Img[i+r][j-r-1].r + ref_Img[i-r-1][j-r-1].r};

            //calculate the average of the pixels in the area
            matRGB[i][j] = {stc_cstI(sum.b/area), stc_cstI(sum.g/area), stc_cstI(sum.r/area)};
        }
    }
}

/*function for the kmeans++ initialization of the centroids*/
void kmeans_pp(vector<vector<bitRGB>> &matRGB, vector<cluster> &centroids, int &numClust){
    vector<std::pair<int,int>> indices(matRGB.size()*matRGB[0].size(),{0,0});
    cluster c;
    bool fNonZero;
    std::random_device rd;
    std::mt19937 gen(rd());

    cout<<" kmeans++"<<mspace;
    
    //randomly select the first centroid
    std::uniform_int_distribution<> distrib(0, matRGB.size());
    std::uniform_int_distribution<> distrib2(0, matRGB[0].size()-1);
    int i = distrib(gen);
    int j = distrib2(gen);
    c = {stc_cstD(matRGB[i][j].ble), stc_cstD(matRGB[i][j].grn), stc_cstD(matRGB[i][j].red)};
    centroids.push_back(c);

    //select the rest of the centroids
    while(centroids.size() < numClust){
        vector<double> distance_distrib(matRGB.size()*matRGB[0].size(),0);
        int k = 0; fNonZero = false;
        for(int i = 0; i < matRGB.size(); i++){
            for(int j = 0; j < matRGB[0].size(); j++, k++){

                for(int l = 0; l < centroids.size(); l++){ 
                    double d = matRGB[i][j].distance(centroids[l]); 
                    if(d < matRGB[i][j].min_dist){ matRGB[i][j].min_dist = d; matRGB[i][j].clust = l; }
                }
                
                //flag to check if the pixel has a distance greater than zero
                if(matRGB[i][j].min_dist > precision) fNonZero = true;
                distance_distrib[k] = matRGB[i][j].min_dist;
                if(!(centroids.size()-1)) indices[k] = {i,j};
            }
        }

        //if all the distances are zero, only create the centroids that have been created
        if(!fNonZero){ cout<<"All pixels are clustered. The program only create: "<<centroids.size()<<" centroids"<<endl; break; }

        //select the next centroid with a probability proportional to the distance to the nearest centroid
        std::discrete_distribution<> weightedDist(distance_distrib.begin(), distance_distrib.end());
        int rndI = weightedDist(gen);
        c = {stc_cstD(matRGB[indices[rndI].first][indices[rndI].second].ble), 
             stc_cstD(matRGB[indices[rndI].first][indices[rndI].second].grn), 
             stc_cstD(matRGB[indices[rndI].first][indices[rndI].second].red)};
        centroids.push_back(c);
    }

    cout<<" Initial Centroids"<<endl;
    ClustersInfo(centroids,1);
}

/*function for the kmeans clustering of the pixels in the image*/
void kmeans(vector<vector<bitRGB>> &matRGB, vector<cluster> &centroids, int &show){
    bool CentroidUpdated = true;

    if(show) cout<<hspace<<" kmeans"<<endl;

    //stop if the centroids don't change with a given an epsilon or the maximum number of iterations is reached
    while(CentroidUpdated && iterations--){
        CentroidUpdated = false;

        if(show){ cout << string(102, '-') << endl; cout<<" Iteration: "<<MAX_ITERARIONS - iterations<<endl; }
        for(int i = 0; i < matRGB.size(); i++){
            for(int j = 0; j < matRGB[0].size(); j++){
                for(int k = 0; k < centroids.size(); k++){
                    double d = matRGB[i][j].distance(centroids[k]);
                    if(d < matRGB[i][j].min_dist){ matRGB[i][j].min_dist = d; matRGB[i][j].clust = k; }
                }
                
                centroids[matRGB[i][j].clust].numPixels++;
                centroids[matRGB[i][j].clust].average.b += matRGB[i][j].ble;
                centroids[matRGB[i][j].clust].average.g += matRGB[i][j].grn;
                centroids[matRGB[i][j].clust].average.r += matRGB[i][j].red;
            }
        }

        if(show){ cout<<" Position centroids: "<<endl; ClustersInfo(centroids,0); cout<<endl<<" New Centroids: "<<endl; header();}


        vector<cluster> oldCentroids = centroids;
        //calculate the new centroids and check if they have changed
        for(int k = 0; k < centroids.size(); k++){
            if(centroids[k].numPixels > 0){
                centroids[k].b = centroids[k].average.b / centroids[k].numPixels;
                centroids[k].g = centroids[k].average.g / centroids[k].numPixels;
                centroids[k].r = centroids[k].average.r / centroids[k].numPixels;
            }
            
            //check if the centroids have changed to continue the algorithm or stop
            if(centroids[k].oldCentroidDistance(oldCentroids[k]) > EPSILON) CentroidUpdated = true;
            if(show) info(centroids[k], k);
        }

        //reset the accumulated values of the centroids
        if(CentroidUpdated){
            for(auto &ctroid: centroids){
                ctroid.numPixels = 0;
                ctroid.average = averageClust();
            }
        }
    }
    
    cout<<hspace<<" Final Centroids"<<endl;
    ClustersInfo(centroids,0);    
    cout<<mspace;
}

#endif