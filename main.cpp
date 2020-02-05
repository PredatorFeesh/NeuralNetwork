#include "headers/createCifar.h"
#include "headers/NeuralNetwork.h"
#include "headers/Matrix.h"
#include <csignal>

NeuralNetwork *net;

void signal_handler( int signum )
{
    if ( signum == 2)
    {
        cout << "Got signal to stop! Saving network." << endl;
        (*net).saveNetwork("cancelSignal.txt");
        cout << "Network saved! Exiting." << endl;
    }
    exit(signum);
}

int main()
{
    signal(SIGINT, signal_handler);

    Cifar::Cifar data(1, 0.5f);
    data.preprocess(Cifar::STANDARD);

    net = new NeuralNetwork(vector<size_t>{data.img_size,2000, 2000, 100, 50, 10}, Xavier, SIGMOID, SOFTMAX, CrossEntropy);
    // NeuralNetwork net("network.txt");

    (*net).train(data, 200, 0.00005);

    // cout << "Starting to forward propogate" << endl;

    // // auto dat = data.get_random_train();

    // // net.forward(dat[0]);
    // // net.computeCost(dat[1]);

    // // net.get_output().print( 1, 10 );

    // // net.saveNetwork("network.txt");

    // // net.updateWeights(dat[1], 0.01);


    return 0;
}