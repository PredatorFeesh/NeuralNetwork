#include "headers/createCifar.h"
#include "headers/NeuralNetwork.h"
#include "headers/Matrix.h"

int main()
{
    Cifar::Cifar data(1);
    data.preprocess(Cifar::STANDARD);

    NeuralNetwork net(vector<size_t>{data.size,500,10}, Xavier, SIGMOID, SOFTMAX, CrossEntropy);

    cout << "Starting to forward propogate" << endl;

    auto dat = data.get_random_train();

    net.forward(dat[0]);
    net.computeCost(dat[1]);

    net.get_output().print( 1, 10 );

    net.saveNetwork("network.txt");

    cout << "done program" << endl;


    return 0;
}