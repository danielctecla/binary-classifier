#ifndef STRUCT_H
#define STRUCT_H

#include <cmath>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cout;
using std::cin;

#define endl "\n"
#define mspace "\n\n"
#define hspace "\n\n\n"

struct averageClust{
    long double b, g, r;

    //constructors
    averageClust() : b(0), g(0), r(0) {}
    averageClust(long double b, long double g, long double r) : b(b), g(g), r(r) {}
};

struct cluster{
    double b;
    double g;
    double r;
    int numPixels;
    struct averageClust average;

    //constructors
    cluster() : b(0), g(0), r(0), numPixels(0), average() {}
    cluster(double b, double g, double r) : b(b), g(g), r(r), numPixels(0), average() {}
    
    //calculate the distance between two clusters
    double oldCentroidDistance(cluster &clst) const {
        return sqrt((clst.b - b)*(clst.b - b) + 
                    (clst.g - g)*(clst.g - g) + 
                    (clst.r - r)*(clst.r - r));
    }
};

struct bitRGB{
    int ble;
    int grn;
    int red;
    int clust;
    double min_dist;

    //constructors
    bitRGB() : ble(0), grn(0), red(0), clust(-1), min_dist(__DBL_MAX__) {}
    bitRGB(int b, int g, int r) : ble(b), grn(g), red(r), clust(-1), min_dist(__DBL_MAX__) {}

    void setColors(int b, int g, int r){
        ble = b;
        grn = g;
        red = r;
    }

    //calculate the approximate distance between two pixels (without sqrt)
    double distance(cluster &clst) const {
        return(clst.b - static_cast<double>(ble))*(clst.b - static_cast<double>(ble)) + 
              (clst.g - static_cast<double>(grn))*(clst.g - static_cast<double>(grn)) + 
              (clst.r - static_cast<double>(red))*(clst.r - static_cast<double>(red));
    }
};

struct data{
    int area; //area
    int perimeter; //perimeter
    int class_; //class

    //constructors
    data() : area(0), perimeter(0), class_(-1) {}
    data(int area, int perimeter, int class_) : area(area), perimeter(perimeter), class_(class_) {}
    
    //calculate the distance between two data points
    double distance(data &second) const{
        return sqrt(pow((static_cast<double>(area)/1000) - (static_cast<double>(second.area)/1000),2) 
                  + pow((static_cast<double>(perimeter)/1000) - (static_cast<double>(second.perimeter)/1000),2));
    }
};

#endif