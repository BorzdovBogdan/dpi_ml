void trainFunc(char* charFileName){
    string fileName((const char*) charFileName);
    typedef matrix<double,2,1> sample_type;

    typedef radial_basis_kernel<sample_type> kernel_type;

    
    std::vector<sample_type> samples;
    std::vector<double> labels;
    sample_type m;
    ifstream infile;
    infile.open(fileName);
    string line;
    string token;
    int counter = 0;
    while(std::getline(infile, line))
    {
        cout<<counter<<endl;
        std::istringstream ss(line);
        std::getline(ss, token, ' ');
        m(0) = std::stoi(token);
        std::getline(ss, token, ' ');
        m(1) = std::stoi(token);
        std::getline(ss, token, ' ');
        
        samples.push_back(m);
        if(token == "0"){
            labels.push_back(+1);
        }else{
            labels.push_back(-1);
        }
        std::getline(ss, token, ' ');
        counter++;

    }
    vector_normalizer<sample_type> normalizer;
    normalizer.train(samples);
    for (unsigned long i = 0; i < samples.size(); ++i)
        samples[i] = normalizer(samples[i]); 

    randomize_samples(samples, labels);

    rvm_trainer<kernel_type> trainer;
    
    trainer.set_epsilon(0.001);
    trainer.set_max_iterations(2000000);
    

    typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;  
    typedef normalized_function<probabilistic_funct_type> pfunct_type;

    pfunct_type learned_pfunct; 
    learned_pfunct.normalizer = normalizer;
    cout<<"1"<<endl;

    learned_pfunct.function = train_probabilistic_decision_function(trainer, samples, labels, 3);
    cout<<"2"<<endl;



    serialize(fileName + "_"+"saved_function.dat") << learned_pfunct;
}
void check(char* charFileName, char* checkFile){
    string fileName((const char*) charFileName);
    string checkFileName((const char*) checkFile);
    typedef matrix<double,2,1> sample_type;
    sample_type m;
    typedef radial_basis_kernel<sample_type> kernel_type;
    typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;  
    typedef normalized_function<probabilistic_funct_type> pfunct_type;

    pfunct_type learned_pfunct; 
    deserialize(fileName+"_"+"saved_function.dat") >> learned_pfunct;
    ifstream infile;
    infile.open(checkFileName);
    string line;
    string token;
    int correct = 0;
    int falsh = 0;
    while(std::getline(infile, line))
    {
        
        std::istringstream ss(line);
        std::getline(ss, token, ' ');
        m(0) = std::stoi(token);
        std::getline(ss, token, ' ');
        m(1) = std::stoi(token);
        std::getline(ss, token, ' ');
        double res = learned_pfunct(m);
        if((token == "0" && res < 0.5) || (token == "1" && res >=0.5)){
            correct++;
        }else if ((token == "0" && res >= 0.5) || (token == "1" && res < 0.5)){
            falsh++;
        }
        std::getline(ss, token, ' ');

    }
    cout<< "Accuracy: correct predictions: " << correct <<" from "<<1000<<endl;
    cout<<"Accuracy: falsh predictions: " << falsh <<" from "<<1000<<endl;
}
/*string fileName(argv[2]);
    typedef matrix<double,2,1> sample_type;
    sample_type m;
    typedef radial_basis_kernel<sample_type> kernel_type;
    typedef probabilistic_decision_function<kernel_type> probabilistic_funct_type;  
    typedef normalized_function<probabilistic_funct_type> pfunct_type;

    pfunct_type learned_pfunct; 
    deserialize(fileName+"_"+"saved_function.dat") >> learned_pfunct;
    
    cout<<"Health probabilities:"<<endl;
    m(0) = 20;
    m(1) = 70;
    cout<<"age = "<<m(0)<<" "<<"weight = "<<m(1)<<endl;
    cout<< "Healthy on " << 100*learned_pfunct(m) <<" %"<< endl;
    cout<<endl;

    m(0) = 20;
    m(1) = 80;
    cout<<"age = "<<m(0)<<" "<<"weight = "<<m(1)<<endl;
    cout<< "Healthy on " << 100*learned_pfunct(m) <<" %"<< endl;
    cout<<endl;

    m(0) = 20;
    m(1) = 90;
    cout<<"age = "<<m(0)<<" "<<"weight = "<<m(1)<<endl;
    cout<< "Healthy on " << 100*learned_pfunct(m) <<" %"<< endl;
    cout<<endl;

    m(0) = 20;
    m(1) = 100;
    cout<<"age = "<<m(0)<<" "<<"weight = "<<m(1)<<endl;
    cout<< "Healthy on " << 100*learned_pfunct(m) <<" %"<< endl;*/