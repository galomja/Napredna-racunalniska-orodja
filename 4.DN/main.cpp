#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>


int main() {
    std::ifstream datoteka("vhodna_datoteka.txt");
    std::string vrstica;
    bool firstLine = true;
    double x;
    double y;
    std::vector<double>xvec;
    std::vector<double>yvec;
    
    
    while(std::getline(datoteka, vrstica,':')){
        if(firstLine){
            firstLine=false;
            continue;
        }
        std::istringstream iss(vrstica);
        iss>>x>>y;
        xvec.push_back(x);
        yvec.push_back(y);
        std::cout<<"x="<<x<<", y="<<y<< std::endl;
    }
    double dx = xvec[1]-xvec[0];
    int N = xvec.size();
    std::vector<double> odvodi;
    double df;
    for(int i = 0;i<=N;++i){
        if(i==0){
            df=(-3*yvec[0]+4*yvec[1]-yvec[2])/(2*dx);
        }
        if(i==N){
            df=(3*yvec[i-1]-4*yvec[i-2]+yvec[i-3])/(2*dx);
        }
        else{
            df=(yvec[i+1]-yvec[i-1])/(2*dx);
        }
        odvodi.push_back(df);
        std::cout<<df<<std::endl;
    }
    std::ofstream datoteka2("diff_poly.txt");
    for(double df : odvodi){
        datoteka2<<df<<"\n";
    }
    datoteka2.close();
    return 0;
}