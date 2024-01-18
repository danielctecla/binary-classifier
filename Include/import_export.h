#ifndef IMPORT_EXPORT_H
#define IMPORT_EXPORT_H

#include <fstream>
#include <random>
#include "struct.h"

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
};

struct BITMAPINFOHEADER {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

#define stc_cst static_cast

void readImgData(std::string &img_name){
    string temp_name;
    img_name = "./images/";
    std::cout<<"Image name: ";
    std::cin>>temp_name;
    img_name += temp_name;
    img_name += ".bmp";
}

void createMatrixRGB(vector<vector<bitRGB>> &matrixRGB, vector<char> &buff){
    //create a matrix to store the image data
    for(int i = 0, pointer = 54; i < matrixRGB.size() ; i++){
        for(int j = 0; j < matrixRGB[0].size() ; j++,pointer += 3){
            matrixRGB[i][j].setColors(stc_cst<int>(stc_cst<unsigned char>(buff[pointer])), 
                                      stc_cst<int>(stc_cst<unsigned char>(buff[pointer + 1])), 
                                      stc_cst<int>(stc_cst<unsigned char>(buff[pointer + 2])));
        }
    }
}

void ClusteredImg(vector<vector<bitRGB>> &matrixRGB, vector<char> &buff, std::streampos &sizeF, vector<cluster> &centroids){
    int pointer = 54;
    char * new_image = new char[sizeF];
    for(int i = 0; i < pointer ; i++) new_image[i] = buff[i];

    for(int i = 0; i < matrixRGB.size() ; i++){
        for(int j = 0; j < matrixRGB[0].size() ; j++, pointer += 3){
            new_image[pointer] = stc_cst<char>(stc_cst<int>(centroids[matrixRGB[i][j].clust].b));
            new_image[pointer + 1] = stc_cst<char>(stc_cst<int>(centroids[matrixRGB[i][j].clust].g));
            new_image[pointer + 2] = stc_cst<char>(stc_cst<int>(centroids[matrixRGB[i][j].clust].r));
        }
    }
    
    std::ofstream outputFile("clustered.bmp", std::ios::binary);
    outputFile.write(new_image, sizeF);
    outputFile.close();

    delete[] new_image;
    cout<<"Image clustered exported"<<endl;
}

void sizeImg(int &sizeV, int &sizeH, std::ifstream &file){
    BMPHeader header;
    BITMAPINFOHEADER infoHeader;

    file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

    cout << "File Size: " << (int)header.fileSize << endl;
    cout << "Width: " << (int)infoHeader.width << endl;
    cout << "Height: " << (int)infoHeader.height << endl;

    sizeV = infoHeader.height;
    sizeH = infoHeader.width;

    cout<<endl<<"Image size: "<<sizeV<<"x"<<sizeH<<endl;
}

void revomeBackground(vector<vector<bitRGB>> &matRGB, vector<cluster> &centroids, vector<char> &buff, std::streampos &sizeF){
    vector<int> corners = {
        matRGB[0][0].clust, 
        matRGB[0][matRGB[0].size() - 1].clust, 
        matRGB[matRGB.size() - 1][0].clust, 
        matRGB[matRGB.size() - 1][matRGB[0].size() - 1].clust
    };

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distrib(0,3);    
    int backG1 = corners[distrib(gen)];
    int backG2 = corners[distrib(gen)];

    int pointer = 54;
    char * new_image = new char[sizeF];
    for(int i = 0; i < pointer ; i++) new_image[i] = buff[i];

    for(int i = 0; i < matRGB.size() ; i++){
        for(int j = 0; j < matRGB[0].size() ; j++, pointer += 3){
            if(matRGB[i][j].clust == backG1 || matRGB[i][j].clust == backG2){
                new_image[pointer] = 255;
                new_image[pointer + 1] = 255;
                new_image[pointer + 2] = 255;
                matRGB[i][j].setColors(255, 255, 255);
            }else{
                new_image[pointer] = 0;
                new_image[pointer + 1] = 0;
                new_image[pointer + 2] = 0;
                matRGB[i][j].setColors(0, 0, 0);
            }
        }
    }

    std::ofstream outputFile("w_background.bmp", std::ios::binary);
    outputFile.write(new_image, sizeF);
    outputFile.close();

    delete[] new_image;
    cout<<"Image without background exported"<<endl;
}

void exportImg(vector<vector<bitRGB>> &matrixRGB, vector<char> &buff, std::streampos &sizeF,string &OutputName){
    int pointer = 54;
    char* new_image = new char[sizeF];
    for(int i = 0; i < pointer ; i++) new_image[i] = buff[i];

    for(int i = 0; i < matrixRGB.size() ; i++){
        for(int j = 0; j < matrixRGB[0].size() ; j++, pointer += 3){
            new_image[pointer] = static_cast<char>(matrixRGB[i][j].ble);
            new_image[pointer + 1] = static_cast<char>(matrixRGB[i][j].grn);
            new_image[pointer + 2] = static_cast<char>(matrixRGB[i][j].red);
        }
    }
    
    std::ofstream outputFile(OutputName, std::ios::binary);
    outputFile.write(new_image, sizeF);
    outputFile.close();

    delete[] new_image;

}

#endif