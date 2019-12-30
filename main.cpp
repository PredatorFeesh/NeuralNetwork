#include "headers/createCifar.h"
#include "headers/NeuralNetwork.h"
#include "headers/Matrix.h"


int main()
{
    NeuralNetwork net(vector<size_t>{5,2,5});
    
    Cifar::Cifar<float> data(1);

    net.forward(vector<float>{1,3,2,1,3});
    net.a[2].print(1,5);

    cout << "done program" << endl;
    return 0;
}