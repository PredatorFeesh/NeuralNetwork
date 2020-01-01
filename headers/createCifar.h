#ifndef CREATE_CIFAR_H
#define CREATE_CIFAR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <sstream>
#include <algorithm>


namespace Cifar
{

    using namespace std;  


    enum ScaleType
    {
        MINMAX,
        STANDARD
    };

    class Cifar
    {

    public:

        double preprocessed=false;

        vector<float> y_train;
        vector<float> X_train;

        vector<float> y_test;
        vector<float> X_test;

        unsigned cifarNumImages = 1000;

        int num_train = cifarNumImages;
        int num_test = 0;

        unsigned long size = 32*32*3;

        Cifar(int num)
        {
            set_cifar(num);
        };
        ~Cifar(){};

        void set_cifar(int num) // X, y
        {
            // Read as such:
            // X -> all stacked on top of one another
            // Y -> also stacked on top of one another

            ifstream cifarfile ("cifar_data/data_batch_"+to_string(num)+".bin",ios::binary);
            
            if (!cifarfile.good()) { cout << "File is not good!" << endl; exit(1); } // Check file good

            cout << endl;
            cout << "Making cifar" << endl;
            for (int w = 0; w < cifarNumImages; w++) 
            {   
                    
                    y_train.push_back( static_cast<uint8_t>(cifarfile.get()));     //binaryToDecimal()
                    for (unsigned i = 0; i < size; ++i) 
                    {
                        X_train.push_back( static_cast<float>(static_cast<uint8_t>(cifarfile.get())));  // binaryToDecimal()
                    }
            }

            cout << "Done making cifar" << endl;

            // return vector<vector<int>>{X_train, y_train};

        }
	
        void preprocess(ScaleType scale)
        {
            // @param scale: specifies the type of scale to apply
            // @return none: updates the values in the batches (train and test)
            preprocessed = true;
            cout << "Starting preprocessing..." << endl;
            
            if( scale == MINMAX )	    
            {
                // const auto [test_min, test_max] = minmax_element( X_test.begin(), X_test.end() );
                // const auto [train_min, train_max] = minmax_element( X_train.begin(), X_train.end());

                // float dtrain = *train_max - *train_min;
                // float dtest = *test_max - *test_min;

                // Max value is 255 and min is 0, so we could just substitute that

                for_each( X_train.begin(), X_train.end(), 
                    [&](float& point){ point = (point - 0) / (255-0); } ); // scale train 
                
                for_each( X_test.begin(), X_test.end(), 
                    [&](float& point){ point = (point - 0) / (255-0); } ); // scale test

            } 
            else if (scale == STANDARD)
            {

                // We need to split the data into each own part
                int start_spot, stop_spot;
                for (size_t image_number = 0; image_number < num_train; image_number++)
                {
                    start_spot = image_number*size;
                    stop_spot = (image_number+1)*size-1;
                    double mean_train = std::accumulate(X_train.begin() + start_spot, X_train.begin() + stop_spot, 0.0) / X_train.size();
                    double accum=0.0;
                    for_each(X_train.begin() + start_spot, X_train.begin() + stop_spot, 
                        [&](const double d) {accum += (d - mean_train) * (d - mean_train); });
                    double stdev_train = sqrt(accum / (size-1)); // Get the standard deviation

                    for_each( X_train.begin() + start_spot, X_train.begin() + stop_spot, 
                        [&](float& point){ point = (point - mean_train) / stdev_train; } ); // update train

                }

                for (size_t image_number = 0; image_number < num_test; image_number++)
                {
                    start_spot = num_train*size;
                    stop_spot = (num_train+1)*size-1;
                    double mean_test = std::accumulate(X_test.begin() + start_spot, X_test.begin() + stop_spot, 0.0) / X_test.size();
                    double accum=0.0;
                    for_each(X_test.begin() + start_spot, X_test.begin() + stop_spot, 
                        [&](const double d) {accum += (d - mean_test) * (d - mean_test); });
                    double stdev_test = sqrt(accum / (size-1)); // Get the standard deviation

                    for_each( X_test.begin() + start_spot, X_test.begin() + stop_spot, 
                        [&](float& point){ point = (point - mean_test) / stdev_test; } ); // update test
                }

            }
            cout << "Finished preprocessing!" << endl;
        }

        void print(vector<float> vec)
        {
            for( float val : vec )
                cout << val << " ";
            cout << endl;
        } 


        vector<float> getonehot(int label, size_t max_labels)
        {
            vector<float> onehot(max_labels);

            for ( int i = 0 ; i < max_labels ; i++ )
                if (label == i)
                    onehot[i]=1;
                else
                    onehot[i]=0;
            
            return onehot;
        }

        vector<float> get_train_onehot(int image_number)
        {
            return getonehot(y_train[image_number], 10);
        } 

        vector<float> get_test_onehot(int image_number)
        {
            return getonehot(y_test[image_number], 10);
        } 

        vector<float> get_train_data(int image_number)
        {
            vector<float> data(size);
            int start_spot = image_number*size;
            for(int i = start_spot; i < (image_number+1)*size; i++)
                data[i - start_spot ] = X_train[i];
            return data;
        }
        
        vector<float> get_test_data(int image_number)
        {
            vector<float> data(size);
            int start_spot = image_number*size;
            for(int i = start_spot; i < (image_number+1)*size; i++)
                data[i - start_spot ] = X_train[i];
            return data;
        }



        vector<vector<float>> get_random_train(int type = 1)
        { // 0 = regular; 1 = onehot

            int randindx = rand() % (cifarNumImages);

            if ( type == 1 )
                return vector<vector<float>> { get_train_data(randindx), get_train_onehot(randindx) };
            else if ( type == 0 )
                return vector<vector<float>> { get_train_data(randindx), vector<float>{y_train[randindx]} };
        }

        vector<vector<float>> get_random_test(int type = 1)
        { // 0 = regular; 1 = onehot

            int randindx = rand() % (cifarNumImages);

            if ( type == 1 )
                return vector<vector<float>> { get_test_data(randindx), get_test_onehot(randindx) };
            else if ( type == 0 )
                return vector<vector<float>> { get_test_data(randindx), vector<float>{y_train[randindx]} };
        }

    };

}






#endif