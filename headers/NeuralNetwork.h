#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "../headers/Matrix.h"
#include "../headers/mathHelper.h"

using namespace std;

enum Activation
{
    SIGMOID
};

enum Final
{
    SOFTMAX,
    SIGMOIDAL
};

enum Cost
{
    CrossEntropy, // @TODO First implemented
    KLDIV // @TODO (Maybe implement)
};

enum WeightInitMethod
{
    LowRandoms, // +- 0.01
    Zero, // 0 for all
    Xavier,// +- sqrt(6) / sqrt(n_{i} + n_{i+1} ) where n is number of nodes in layer i
    Kaiming // @TODO: GOOD FOR DEEPER NETWORKS:  
};

class NeuralNetwork
{

    public:

        Activation activfunc;
        Final finalfunc;
        Cost costfunc;

        // This holds all the matricies of weights.
        // Can understand like this:
        // w[i][j][k] is the weight in the ith layer connecting node k in i-i to j in layer i.
        vector<Matrix> w; 

        // This is the change we will do to w
        vector<Matrix> nab_w;
        
        // This is the bias. b[i] given bias of layer [i]
        vector<Matrix> b;

        // This is the change to be applied to the bias vector
        vector<Matrix> nab_b;

        // This is the layer before activation is applied
        // z[l][i][k] is the lth layer val i from k in layer l-1
        vector<Matrix> z;

        // This is the layer after activation is applied
        // a[l][i][k] is the lth layer val i from k in layer l-1
        vector<Matrix> a;

        // This is the current cost of our system (after a forward pass)
        float cost = 0.0f;


        // This is the topolgy of our neural network (how layers are constructed)
        vector<size_t> top;
        size_t net_size;

        NeuralNetwork(vector<size_t> topology, WeightInitMethod initmet, Activation activfunc, Final finalfunc, Cost costfunc);

        Matrix get_output(){ return a[net_size]; };

        void forward(Matrix X); // input
        void forward(vector<float> X); // input
    
        void computeCost(Matrix y); // This assumes we already forward propogated - calculated based on that
        void computeCost(vector<float> y);
        float getCost(){ return cost; }

        void updateWeights(Matrix y);



};

#endif