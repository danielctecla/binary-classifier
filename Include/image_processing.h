#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <fstream>

#include "struct.h"
#include "import_export.h"
#include "kmeans.h"

void image_processing(vector<vector<bitRGB>> &matRGB, vector<char> &buffer, std::streampos &sizeF){
    int numClust = 3;
    int show = 0, kernel = 3;
    vector<cluster> centroids;

    //create the matrix of the image
    createMatrixRGB(matRGB, buffer);


    cout<<"Show clustering? (1/0): "; cin>>show;
    cout<<mspace;

    /* Reduce the noise and apply kmeans */
    Box_Blur(matRGB, kernel);
    kmeans_pp(matRGB, centroids, numClust);
    kmeans(matRGB, centroids, show);
    ClusteredImg(matRGB, buffer, sizeF, centroids);

    /* Remove the background */
    revomeBackground(matRGB, centroids, buffer, sizeF);    
}

#endif