#include <fstream>

#include "./include/struct.h"
#include "./include/import_export.h"
#include "./include/image_processing.h"
#include "./include/knn.h"

void clearScreen();

int main(){
    clearScreen();

    int sizeV,sizeH;
    string img_name, Classification;
    string nameOutput = "perimeter.bmp";

    /* Import image's information */
    readImgData(img_name);
    std::ifstream file (img_name, std::ios::binary );
    if(!file.is_open()){ cout<<"Error opening file"<<endl; return -1; }
    sizeImg(sizeV, sizeH, file);
    vector<vector<bitRGB>> matRGB(sizeV, vector<bitRGB>(sizeH)); 
    file.seekg(0, std::ios::end); 
    std::streampos sizeF = file.tellg();
    file.seekg(0, std::ios::beg);
    vector<char> buffer(sizeF);
    file.read(buffer.data(), sizeF);
    if (file.fail()){ cout << "Error reading file" << endl; return -1; }

    /* call image processing function*/
    image_processing(matRGB, buffer, sizeF);  

    /*get data from image as area and perimeter*/
    int perimeter, area;
    std::tie(area, perimeter) = getData(matRGB);
    exportImg(matRGB, buffer, sizeF, nameOutput);
    cout<<mspace<<"Parameters of the image: "<<endl;
    cout<<"Area: "<<area<<endl;
    cout<<"Perimeter: "<<perimeter<<endl;

    /* call knn function and send the parameters extracted from the image*/
    cout<<mspace<<"Binary Classification"<<endl;
    Classification = knn(area, perimeter); 

    cout<<mspace<<"The model classified the image as: "<<Classification<<endl;
    file.close();
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}