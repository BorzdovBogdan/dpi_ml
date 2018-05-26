#include <fstream>
#include <iostream>
#include <vector>
#include <dlib/svm.h>
#include <dlib/map.h>
using namespace std;
using namespace dlib;

void trainFunc(char* charFileName){
    string fileName (charFileName);
    typedef matrix<double,11,1> sample_type;

    typedef radial_basis_kernel<sample_type> kernel_type;

    
    std::vector<sample_type> samples;
    std::vector<double> labels;
    sample_type m(4);
    ifstream infile;
    infile.open(fileName);
    string line;
    string token;
    while(std::getline(infile, line))
    {
        
        std::istringstream ss(line);
        std::getline(ss, token, ' ');
        char* charStr = new char[token.length()+1];
        strcpy(charStr,token.c_str());
        for (int i = 0; i < token.length(); ++i)
        {
            m(i) = charStr[i];
        }
        std::getline(ss, token, ' ');
        m(9) = std::stoi(token);
        std::getline(ss, token, ' ');
        m(10) = std::stoi(token);
        std::getline(ss, token, ' ');
        
        samples.push_back(m);
        if(token == "0"){
            labels.push_back(+1);
        }else{
            labels.push_back(-1);
        }
        std::getline(ss, token, ' ');

    }
    
    vector_normalizer<sample_type> normalizer;
    normalizer.train(samples);
    for (unsigned long i = 0; i < samples.size(); ++i)
        samples[i] = normalizer(samples[i]); 


    randomize_samples(samples, labels);


    rvm_trainer<kernel_type> trainer;

    
    trainer.set_epsilon(0.001);
    
    trainer.set_max_iterations(2000);
    

    typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;  
    typedef normalized_function<probabilistic_funct_type> pfunct_type;

    pfunct_type learned_pfunct; 
    learned_pfunct.normalizer = normalizer;
    learned_pfunct.function = train_probabilistic_decision_function(trainer, samples, labels, 3);

    serialize(fileName+"_"+"name_saved_function.dat") << learned_pfunct;
}
int main(int argc, char** argv){
    if(argc > 1){
        trainFunc(argv[1]);
    }

    typedef matrix<double,11,1> sample_type;
    sample_type m(4);
    typedef radial_basis_kernel<sample_type> kernel_type;
    typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;  
    typedef normalized_function<probabilistic_funct_type> pfunct_type;

    pfunct_type learned_pfunct; 

    getchar();
    deserialize("name_saved_function.dat") >> learned_pfunct;
    string name1 = "Wilmer";
    char* charStr = new char[name1.length()+1];
    strcpy(charStr,name1.c_str());
    cout<<"Health probabilities:"<<endl;
    cout<<"Name: ";
    for (int i = 0; i < name1.length(); ++i)
    {
        m(i) = charStr[i];
        cout<<charStr[i];
    }
    cout<<endl;
    m(9) = 35;
    m(10) = 80;
    cout<<"age = "<<m(9)<<" "<<"weight = "<<m(10)<<endl;

    cout<< "Healthy  = " << 100*learned_pfunct(m) << endl;
}



