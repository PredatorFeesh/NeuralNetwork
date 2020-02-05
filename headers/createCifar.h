#ifndef CREATE_CIFAR_H
#define CREATE_CIFAR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <sstream>
#include <algorithm>
#include <iterator>

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
        double split=false;

        vector<float> X;
        vector<float> y;


        unsigned cifarNumImages = 0;

        int num_train = 0;
        int num_test = 0;

        unsigned img_size = 32*32*3;

        Cifar(int num, float train_test_ratio)
        {
            set_cifar(num);
            train_test_seperate(train_test_ratio);
        };
        ~Cifar(){};

        void train_test_seperate(float train_test_ratio)
        {
            if (split)
            {
                cout << "Can't split again! @TODO" << endl;
                return;
            }
            num_train = cifarNumImages * train_test_ratio;
            num_test = cifarNumImages - num_train;

            cout << "Num train: " << num_train << endl;
            cout << "Num test: " << num_test << endl;

            split=true;

        }

        void set_cifar(int num) // X, y
        {
            // Read as such:
            // X -> all stacked on top of one another
            // Y -> also stacked on top of one another

            ifstream cifarfile ("cifar_data/data_batch_"+to_string(num)+".bin",ios::binary);

            if (!cifarfile.good()) { cout << "File is not good!" << endl; exit(1); } // Check file good

            cout << endl;
            cout << "Making cifar" << endl;
            while (!cifarfile.eof())
            {

                    y.push_back( static_cast<uint8_t>(cifarfile.get()));     //binaryToDecimal()
                    for (unsigned i = 0; i < img_size; ++i)
                    {
                        X.push_back( static_cast<float>(static_cast<uint8_t>(cifarfile.get())));  // binaryToDecimal()
                    }
                cifarNumImages++;
            }

            cout << "Done making cifar" << endl;

            // return vector<vector<int>>{X, y};

        }

        void preprocess(ScaleType scale)
        {
            // @param scale: specifies the type of scale to apply
            // @return none: updates the values in the batches (train and test)
            preprocessed = true;
            cout << "Starting preprocessing..." << endl;

            if( scale == MINMAX )
            {
                // const auto [test_min, test_max] = minmax_element( X.begin(), X.end() );
                // const auto [train_min, train_max] = minmax_element( X.begin(), X.end());

                // float dtrain = *train_max - *train_min;
                // float dtest = *test_max - *test_min;

                // Max value is 255 and min is 0, so we could just substitute that

                for_each( X.begin(), X.end(),
                    [&](float& point){ point = (point - 0) / (255-0); } ); // scale train

                for_each( X.begin(), X.end(),
                    [&](float& point){ point = (point - 0) / (255-0); } ); // scale test

            }
            else if (scale == STANDARD)
            {

                // We need to split the data into each own part
                int start_spot, stop_spot;

                double mean, accum, std_dev = 0.0f;

                for (size_t image_number = 0; image_number < num_test; image_number++)
                {
                    start_spot = num_train*img_size;
                    stop_spot = (num_train+1)*img_size-1;
                    mean = std::accumulate(X.begin() + start_spot, X.begin() + stop_spot, 0.0) / (stop_spot - start_spot);
                    accum=0.0;
                    for_each(X.begin() + start_spot, X.begin() + stop_spot,
                        [&](const double d) {accum += (d - mean) * (d - mean); });
                    double std_dev = sqrt(accum / (img_size-1)); // Get the standard deviation

                    for_each( X.begin() + start_spot, X.begin() + stop_spot,
                        [&](float& point){ point = (point - mean) / std_dev; } ); // update test
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
            return getonehot(y[image_number], 10);
        }

        vector<float> get_test_onehot(int image_number)
        {
            return getonehot(y[image_number], 10);
        }

        vector<float> get_train_data(int image_number)
        {
            vector<float> data(img_size);
            int start_spot = image_number*img_size;
            for(int i = start_spot; i < (image_number+1)*img_size; i++)
                data[i - start_spot ] = X[i];
            return data;
        }

        vector<float> get_test_data(int image_number)
        {
            vector<float> data(img_size);
            int start_spot = (num_train + image_number)*img_size;
            for(int i = start_spot; i < (image_number+1)*img_size; i++)
                data[i - start_spot ] = X[i];
            return data;
        }


        vector<vector<float>> get_random_train( int type = 1)
        { // 0 = regular; 1 = onehot

            int randindx = 0;

            randindx = rand() % (num_train);

            if ( type == 1 )
                return (vector<vector<float>> { get_train_data(randindx), get_train_onehot(randindx) });
            else if ( type == 0 )
                return (vector<vector<float>> { get_train_data(randindx), vector<float>{y[randindx]} });
            else
            {
                cout << "Trying to get type" << type << ": NOT POSSIBLE." << endl;
                return vector<vector<float>>{};
            }
        }

        vector<vector<float>> get_random_test( int type = 1)
        { // 0 = regular; 1 = onehot

            int randindx = 0;

            randindx = rand() % (num_test);

            randindx += num_train; // Since we have an array structured [train, test]

            if ( type == 1 )
                return (vector<vector<float>> { get_test_data(randindx), get_test_onehot(randindx) });
            else if ( type == 0 )
                return (vector<vector<float>> { get_test_data(randindx), vector<float>{y[randindx]} });
            else
            {
                cout << "Trying to get type" << type << ": NOT POSSIBLE." << endl;
                return vector<vector<float>>{};
            }
        }

    };

}



#endif