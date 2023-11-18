#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>

float arctan(float x,int n=10)//definicija funkcije arctan
/*x je argument funkcije, n pa je ševilo členov, 
ki jih želimo uporabiti v taylorjevem razvoju*/
{
    float vsota = 0;
    for(int i=0;i<n;i++){
        vsota += pow(-1,i)*pow(x,2*i+1)/(2*i+1);
    }
    return vsota;
}

//definicija podintegralske funkcije:
float f(float x){
    return exp(3*x)*arctan(x/2);    
};

int main()
{
    //najprej pripravimo vektor točk od a do b:
    int num_points = 600;
    float b = M_PI/4;
    float a = 0;
    float h = (b-a)/(num_points-1);
    std::vector<float> x(num_points);
    for(int i = 0; i < num_points; i++){
        x[i]= a+i*h;
    };   
        
    //pripravimo vektor vrednosti podintegralske funkcije v točkah:
    std::vector<float> fi;
    for(float xi : x){
        fi.push_back(f(xi));
    }
    
    //izračunamo vrednost integrala po podani trapezni formuli:
    float sum;
    
    for(float fk : fi){
        sum += 2*fk;
    };
    sum = sum-fi[0]-fi[num_points-1];
    
    float integral = (b-a)/(2*num_points)*sum;

std::cout<<integral<<std::endl;
    return 0;
}

