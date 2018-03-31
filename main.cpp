
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include "dlib/svm.h"
#include <sstream>
#include "stdlib.h"
using namespace dlib;
using namespace std;

/*void csvWriter(int count){
    std::ofstream myfile;
    std::string s("checkData"+std::to_string(count));
    myfile.open (s);

    srand(time(0));
    for (int i = 0; i < count; ++i) {

        int weight = rand() % 100;
        int age = rand() % 100;

        myfile << std::to_string(age)<<" "<<std::to_string(weight)<<" ";
        bool isDiseased = false;

        if(weight <= 30 && weight>=15 && age <= 10 && age>=5){
            isDiseased = false;
        } else if(weight > 30 && age <= 10){
            isDiseased = true;
        } else if(weight <= 50 && age <= 15 && age > 10){
            isDiseased = false;
        } else if(weight <= 40 && age > 15){
            isDiseased = true;
        } else if(weight >= 60 && weight <= 80 && age <= 80 && age > 18){
            isDiseased = false;
        } else if(weight > 80 && age <= 25){
            isDiseased = true;
        } else if(weight > 90 && age >= 40){
            isDiseased = true;
        }else if(weight < 90 && age >= 40){
            isDiseased = false;
        }else if(weight > 80 && age > 60){
            isDiseased = true;
        }
        if(isDiseased){
            myfile<<"1\n";
        } else{
            myfile<<"0\n";
        }
    }
    myfile.close();
}*/

void predictor(string file)
{

    typedef matrix<double, 2, 1> sample_type;


    // This is a typedef for the type of kernel we are going to use in this example.
    // In this case I have selected the radial basis kernel that can operate on our
    // 2D sample_type objects
    typedef radial_basis_kernel<sample_type> kernel_type;

    // Here we create an instance of the pegasos svm trainer object we will be using.
    svm_pegasos<kernel_type> trainer;
    // Here we setup the parameters to this object.  See the dlib documentation for a
    // description of what these parameters are.
    trainer.set_lambda(0.00001);
    trainer.set_kernel(kernel_type(0.005));

    // Set the maximum number of support vectors we want the trainer object to use
    // in representing the decision function it is going to learn.  In general,
    // supplying a bigger number here will only ever give you a more accurate
    // answer.  However, giving a smaller number will make the algorithm run
    // faster and decision rules that involve fewer support vectors also take
    // less time to evaluate.
    trainer.set_max_num_sv(10);

    std::vector<sample_type> samples;
    std::vector<double> labels;

    // make an instance of a sample matrix so we can use it below
    sample_type sample;


    // Now let's go into a loop and randomly generate 1000 samples.
    ifstream infile;
    infile.open(file);
    string line;
    string token;
    int i = 0;
    while(std::getline(infile, line))
    {
        std::istringstream ss(line);
        std::getline(ss, token, ' ');
        sample(0,0) = std::stoi(token);
        std::getline(ss, token, ' ');
        sample(0,1) = std::stoi(token);

        samples.push_back(sample);
        std::getline(ss, token, ' ');
        if(token == "0"){
            trainer.train(sample,+1);
            labels.push_back(+1);
        }else{
            trainer.train(sample,-1);
            labels.push_back(-1);
        }
    }
    infile.close();

    sample(0,0) = 30;
    sample(0,1) = 70;
    cout<<trainer(sample);
    // At this point we have obtained a decision function from the above batch mode training.
    // Now we can use it on some test samples exactly as we did above.
}

int main(){
    //csvWriter(10);
    //csvWriter(100);
    //csvWriter(1000);
    predictor("data1000");
    return 0;
}