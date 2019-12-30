#include "../headers/NeuralNetwork.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

NeuralNetwork::NeuralNetwork(vector<size_t> top)
{
    top = top;
    net_size = top.size()-1;
    
    // Now we need to make random values to fit into our weights for each layer.
    // We will have topology.size()-1 layers
    for ( unsigned i = 0; i < net_size; i++ )
    {
        b.push_back( Matrix(1, top[i+1]) );
        w.push_back( Matrix(top[i], top[i+1]) );
    }

    // Now we do the bias, z, and a as random just as initialize
    for( size_t layer : top )
    { 
        z.push_back( Matrix( 1 , layer) );
        a.push_back( Matrix( 1 , layer ) );
    }

}


void NeuralNetwork::forward(Matrix X)
{
    z[0], a[0] = X, X;
    for(unsigned i = 1; i < net_size; i++)
    {
        z[i] = (a[i-1]*w[i-1]) + b[i];
        a[i] = z[i].apply(maths::sigmoid);
    }
}

void NeuralNetwork::forward(vector<float> X)
{
    Matrix x(1, X.size());
    x.m_matrix[0] = X;
    forward(x);
}