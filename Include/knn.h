#ifndef KNN_H
#define KNN_H

#include <fstream>
#include <sstream>
#include <algorithm>
#include "struct.h"

//function to validate if the pixel is not in the perimeter
bool is_valid(vector<vector<bitRGB>> &matAux, int i, int j){
    vector<int> x = {0, 0, 1, 1, 1, -1, -1, -1};
    vector<int> y = {1, -1, 0, 1, -1, 0, 1, -1};
    for(int k = 0; k < 8; k++){
        if(matAux[i + x[k]][j + y[k]].ble != 0){
            return false;
        }
    }

    return true;
}

//function to get the number of black pixels and the number of black pixels in the perimeter
std::pair<int,int> getData(vector<vector<bitRGB>> &matRGB){
    int count = 0;
    int count_perimeter = 0;

    vector<vector<bitRGB>> matRGB_aux = matRGB;
    for(int i = 1; i < matRGB.size() - 1; i++){
        for(int j = 1; j < matRGB[0].size() - 1; j++){
            if(matRGB[i][j].ble == 0){
                count++;
                if(is_valid(matRGB_aux, i, j)){
                    //the pixel is not in the perimeter
                    matRGB[i][j].setColors(255, 255, 255);
                    count_perimeter++;
                }
            }
        }
    }

    /*return the area and the perimeter*/
    return {count, count - count_perimeter};
}

void importDataSet(vector<data> &dataSet){
    string dataSetF = "./Images/Dataset/dataset.csv";
    std::ifstream file (dataSetF, std::ios::binary );

    if(!file.is_open()){ cout<<"Error opening file"<<endl; return; }
    string line;

    while(std::getline(file, line)){
        std::stringstream ss(line);
        string token;
        vector<int> row;

        while(std::getline(ss, token, ',')){
            row.push_back(std::stoi(token));    
        }
        
        dataSet.push_back(data(row[0], row[1], row[2]));
    }

    file.close();
}

string knn(int area, int perimeter){
    vector<data> dataSet;
    importDataSet(dataSet);
    data Input(area, perimeter, -1);
    int classWasher, classCoin;
    vector<std::pair<long double,int>> dist;

    classWasher = classCoin = 0;   

    double distance;
    for(int i = 0; i < dataSet.size(); i++){
        std::pair<long double,int> p;
        distance = dataSet[i].distance(Input);
        p.first = distance;
        p.second = dataSet[i].class_;
        dist.push_back(p);
    }
    
    std::sort(dist.begin(), dist.end());

    int showAll = 0, showTo;
    cout<<"show all distances? (1/0): ";
    cin>>showAll;
    if(showAll) showTo = dist.size();
    else showTo = 3;

    cout<<mspace<<"Sorted distances: "<<endl;
    cout<<std::setw(15)<<"Distance"<<std::setw(8)<<"Class"<<endl;
    for(int i = 0; i < showTo; i++){
        cout<<std::setw(15)<<dist[i].first<<std::setw(8)<<dist[i].second<<endl;
    }

    for(int i = 0; i < 3; i++){
        if(dist[i].second == 1) classWasher++;
        else classCoin++;
    }

    return classWasher > classCoin ? "Washer" : "Coin";
}


#endif