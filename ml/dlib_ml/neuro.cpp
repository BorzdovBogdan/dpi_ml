#include <fstream>
#include <iostream>
#include <vector>
#include <dlib/svm.h>
#include <dlib/map.h>
#include <dlib/mlp.h>
using namespace std;
using namespace dlib;


void mlpTrain(char* fileName){
    typedef matrix<double, 2, 1> sample_type;
    sample_type m;
    mlp::kernel_1a_c net(2,5);
    ifstream infile;
    infile.open(fileName);
    string line;
    string token;
    while(std::getline(infile, line))
    {
        std::istringstream ss(line);
        std::getline(ss, token, ' ');
        m(0) = std::stoi(token);
        std::getline(ss, token, ' ');
        m(1) = std::stoi(token);
        std::getline(ss, token, ' ');
        if(token == "0"){
            net.train(m,0);
        }else{
            net.train(m,1);
        }
        std::getline(ss, token, ' ');
    }
    serialize("_neuro.dat") << net;
}
void checkMlp(char* fileName){
    typedef matrix<double, 2, 1> sample_type;
    sample_type m; 
    mlp::kernel_1a_c net(2,5);
    deserialize("_neuro.dat") >> net;
    int falsh = 0;
    int correct = 0;
    ifstream infile;
    infile.open(fileName);
    string line;
    string token;
    double res = 0;
    while(std::getline(infile, line))
    {
        std::istringstream ss(line);
        std::getline(ss, token, ' ');
        m(0) = std::stoi(token);
        std::getline(ss, token, ' ');
        m(1) = std::stoi(token);
        std::getline(ss, token, ' ');
        res = net(m);
        if((token == "0" && res < 0.5) || (token == "1" && res >=0.5)){
            correct++;
        }else if ((token == "0" && res >= 0.5) || (token == "1" && res < 0.5)){
            falsh++;
        }
        std::getline(ss, token, ' ');
    }
    cout<<"correct: "<<correct<<endl;
}
int main(int argc, char** argv){
    cout<<"Input: t age_weight_1000 check_strain_1000"<<endl;
    if(argc > 2 && argv[1][0] == 't'){
        mlpTrain(argv[2]);
        checkMlp(argv[3]);
    }
    return 0;
}