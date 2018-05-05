#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

void exampleWriter(int count, bool isName){
    std::ofstream myfile;
    std::string s("strain_"+std::to_string(count));
    myfile.open (s);
    vector<string> names = {
    "Abram",
    "Burton",
    "Columbus",
    "Lenard",
    "Rolando",
    "Dolores",
    "Shayne",
    "Dion",
    "Lea",
    "Julia",
    "Leila",
    "Drew",
    "Priscilla",
    "Wilmer",
    "Alvin"
    };
    srand(time(0));
    for (int i = 0; i < count; ++i) {
        int name = std::rand() % 15;
        int weight = 0;
        weight = std::rand() % 200;
        /*while(weight < 40){
            weight = std::rand() % 100;
        }*/
        int age = 0;
        while(age < 15){
            age = std::rand() % 70;
        } 
        bool isDiseased = false;
        if(isName){
            myfile<<names[name]<<" "<<std::to_string(age)<<" "<<std::to_string(weight)<<" ";
        }else{
            myfile<<std::to_string(age)<<" "<<std::to_string(weight)<<" ";
        }
        if(weight > 20 && age < 5){
            isDiseased = true;

        }else if(weight < 30 && age > 15){
            isDiseased= true;
        }else if(weight < 10 && age > 3){
            isDiseased=true;
        }else if(weight > 40 && age >= 5 && age < 10){
            isDiseased = true;
        }else if(weight > 70 && age >= 10 && age < 15){
            isDiseased = true;
        }else if(weight > 80 && age >= 15 && age < 20){
            isDiseased = true;
        }else if(weight < 20 && age > 5){
            isDiseased = true;
        }else if(weight < 40 && age >= 10 && age < 15){
            isDiseased = true;
        }else if(weight > 90){
            isDiseased = true;
        }else if(weight < 50 && age > 18){
            isDiseased = true;
        }else if(weight > 90){
            isDiseased = true;
        }
        if(!isDiseased){
            //myfile <<std::to_string(age)<<" "<<std::to_string(weight)<<" "<<names[name]<<"\n";
            myfile<<0<<"\n";
        }else{
            myfile<<1<<"\n";
            //i--;
        }
    }
    myfile.close();
}
int main(int argc, char** argv){
    bool isName = false;
    if(argc > 1 && argv[1][0] == 'n'){
        isName = true;
    }
	exampleWriter(10,isName);
    exampleWriter(100,isName);
    exampleWriter(1000,isName);
	return 0;
}