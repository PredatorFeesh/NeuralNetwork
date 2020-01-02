#include "headers/createCifar.h"
#include "headers/NeuralNetwork.h"
#include "headers/Matrix.h"

int main()
{
    Cifar::Cifar data(1);
    data.preprocess(Cifar::MINMAX);

    NeuralNetwork net(vector<size_t>{data.size,500,10}, SIGMOID, SOFTMAX, CrossEntropy);
    
    cout << "Starting to forward propogate" << endl;

    net.forward(data.get_random_train()[0]);

    net.get_output().print( 1, 10 );

    cout << "done program" << endl;


    return 0;
}