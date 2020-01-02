#include "../headers/NeuralNetwork.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::exp;

NeuralNetwork::NeuralNetwork(vector<size_t> top, Activation activfunc, Final finalfunc, Cost costfunc)
: activfunc(activfunc), finalfunc(finalfunc), costfunc(costfunc)
{
    top = top;
    net_size = top.size()-1;

    w.reserve(net_size);
    b.reserve(net_size);

    z.reserve(net_size+1);
    a.reserve(net_size+1);

    // Now we need to make random values to fit into our weights for each layer.
    // We will have topology.size()-1 layers
    for ( unsigned i = 0; i < net_size; i++ )
    {
        b.push_back( Matrix(1, top[i]) );
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
    // This is for input layer
    z[0], a[0] = X, X; 

    cout << "a[0]" << endl;
    a[0].print(1,10);

    // This is for hidden layers
    for(unsigned i = 1; i < net_size; i++)
    { 
        z[i] = a[i-1]*w[i-1];
        // z[i] += b[i];
        if (activfunc == SIGMOID)
            a[i] = z[i].apply(maths::sigmoid);

        cout << "z["<<i<<"]" << endl;
        z[i].print(1,10);
        cout << "a["<<i<<"]" << endl;
        a[i].print(1,10);
    }

    // This is for the final layer
    z[net_size] = a[net_size-1]*w[net_size-1];
    // z[net_size] += b[net_size];
    if ( finalfunc == SOFTMAX )
    { 
        a[net_size] = z[net_size].apply(maths::expon) / maths::expsum( z[net_size].m_matrix[0] );
    }
    else if ( finalfunc == SIGMOIDAL )
    {
        a[net_size] = z[net_size].apply(maths::sigmoid);
    }

    cout << "Debug::: SOFTMAX V SIGMOIDAL" << endl;
    cout << "Original: " << endl;
    z[net_size].print(1,10);
    cout << "SOFTMAX: " << endl;
    (z[net_size].apply(maths::expon) / maths::expsum(z[net_size].m_matrix[0])).print(1,10);
    cout << "SIGMOIDAL: " << endl;
    z[net_size].apply(maths::sigmoid).print(1,10);
    cout << "END-DEBUG" << endl;
}

void NeuralNetwork::forward(vector<float> X)
{
    Matrix x(1, X.size());
    x.m_matrix[0] = X;
    forward(x);
}

void computeCost(Cost type)
{
    switch (type)
    {
    case CrossEntropy:
        


        break;
    
    default:
        break;
    }
}

