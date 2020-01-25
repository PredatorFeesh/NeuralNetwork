#include "headers/createCifar.h"
#include "headers/NeuralNetwork.h"
#include "headers/Matrix.h"

int main()
{
    Cifar::Cifar data(1);
    data.preprocess(Cifar::MINMAX);

    NeuralNetwork net(vector<size_t>{data.size,2000, 2000, 100, 50, 10}, Xavier, SIGMOID, SOFTMAX, CrossEntropy);
    // NeuralNetwork net("network.txt");

    net.train(data, 200, 0.005);

    // cout << "Starting to forward propogate" << endl;

    // // auto dat = data.get_random_train();

    // // net.forward(dat[0]);
    // // net.computeCost(dat[1]);

    // // net.get_output().print( 1, 10 );

    // // net.saveNetwork("network.txt");

    // // net.updateWeights(dat[1], 0.01);


    return 0;
}