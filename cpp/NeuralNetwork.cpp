#include "../headers/NeuralNetwork.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::exp;

NeuralNetwork::NeuralNetwork(vector<size_t> topology, WeightInitMethod initmet, Activation activfunc, Final finalfunc, Cost costfunc, bool init_done)
: initmet(initmet), activfunc(activfunc), finalfunc(finalfunc), costfunc(costfunc)
{
    top = topology;
    net_size = top.size()-1;

    w.reserve(net_size);
    b.reserve(net_size);
    
    nab_w.reserve(net_size);
    nab_b.reserve(net_size);

    z.reserve(net_size+1);
    a.reserve(net_size+1);

    // Now we need to make random values to fit into our weights for each layer.
    // We will have topology.size()-1 layers
    for ( unsigned i = 0; i < net_size; i++ )
    {

        b.push_back( Matrix(1, top[i]) );
        w.push_back( Matrix(top[i], top[i+1]) );
        
        // Instantiate Weights
        
        switch(initmet)
        {
            case Zero: w[i].initZero(); break;
            case LowRandoms: w[i].initLowRandoms(); break;
            case Xavier: w[i].initXavier(top[i], top[i+1]); break;
            case Kaiming: w[i].initKaming(); break;
        }
        
        nab_b.push_back( Matrix(1, top[i]) );
        nab_w.push_back( Matrix(top[i], top[i+1]) );
        
    }

    // Now we do the bias, z, and a as random just as initialize
    for( size_t layer : top )
    { 
        z.push_back( Matrix( 1 , layer) );
        a.push_back( Matrix( 1 , layer ) );
    }

}

NeuralNetwork::NeuralNetwork(string fromfile)
{
    cout << "Starting Neural Network from file init: " << fromfile << endl;
    fstream file;
    file.open(fromfile, ios::in);

    if (!file.good()) {cout << "This file is bad! Stopping!" << endl; exit(0);}

    string cur;

    file >> cur;
    if (cur == "initmet")
    {
        file >> cur;
        initmet = WeightInitMethod( stoi(cur) );
    } file >> cur;
    if (cur == "activfunc")
    {
        file >> cur;
        activfunc = Activation( stoi(cur) );
    } file >> cur;
    if (cur == "finalfunc")
    {
        file >> cur;
        finalfunc = Final( stoi(cur) );
    } file >> cur;
    if (cur == "costfunc")
    {
        file >> cur;
        costfunc = Cost( stoi(cur) );
    } 

    // Init topology
    file >> cur;
    if( cur == "topology" )
    {
        file >> cur;
        while( cur != "weight" )
        {
            top.push_back(stoi(cur));
            file >> cur;
        }
    }

    net_size = top.size()-1;

    cout << "Initializing " << cur << endl;
    //Init weight
    while( cur != "bias" )
    {
        int i, j, rows, cols;
        file >> j >> rows >> cols;
        Matrix wtemp(rows, cols);

        cout << "Currently doing weight " << i << " :: Dimensions :: " << rows << "x" << cols << endl;
        cout << "Num elements in weight: " << rows*cols << endl;

        for( i = 0; i < rows*cols; i++ ) // Yes we overwrite i
        {
            file >> cur;
            wtemp[ (int)(i/cols) ][ (int)(i%cols) ] = stof(cur);
        }
        w.push_back(wtemp);
        nab_w.push_back( Matrix(top[j], top[j+1]) );
        file >> cur;

        cout << "At end of weights: " << cur << endl;
    }

    cout << "Initializing bias" << endl;

    //Init bias
    while( cur != "0" )
    {
        int i, j, rows, cols;
        file >> j >> rows >> cols;
        Matrix btemp(rows, cols);

        cout << "Currently doing bias " << i << " :: Dimensions :: " << rows << "x" << cols << endl;

        for( i = 0; i < rows*cols; i++ ) // Yes we overwrite i
        {
            file >> cur;
            btemp[ (int)(i/cols) ][ (int)(i%cols) ] = stof(cur);
        }
        b.push_back(btemp);
        nab_b.push_back(Matrix(1, top[j]));
        file >> cur;

        cout << "At end of bias: " << cur << endl;

    }

    file.close();

    for( size_t layer : top )
    { 
        z.push_back( Matrix( 1 , layer) );
        a.push_back( Matrix( 1 , layer ) );
    }

    

}

void NeuralNetwork::saveNetwork(string tofile)
{
    fstream file;

    file.open(tofile, ios::out);

    if( file.is_open() )
    {
        // init params
        file << "initmet " << initmet << endl;
        file << "activfunc " << activfunc << endl;
        file << "finalfunc " << finalfunc << endl;
        file << "costfunc " << costfunc << endl;

        // topology
        file << "topology ";
        for(unsigned i=0; i<top.size(); i++)
            file << top[i] << " ";
        file << endl;

        // save weights
        for(unsigned i=0; i<net_size; i++)
        {
            /// Saved as: weight layer rows cols w[1][1] w[1][2] ... w[1][cols] w[2][1] ...
            file << "weight " << i << " " << w[i].rows << " " << w[i].cols << " ";
            for(unsigned j=0; j<w[i].rows; j++)
                for(unsigned k=0; k<w[i].cols; k++)
                    file << w[i][j][k] << " ";
            file << endl;
        }

        // save bias
        for(unsigned i=0; i<net_size; i++)
        {
            file << "bias " << i << " " <<b[i].rows << " " << b[i].cols << " ";
            for(unsigned j=0; j<b[i].rows; j++)
                for(unsigned k=0; k<b[i].cols; k++)
                    file << b[i][j][k] << " ";
            file << endl;
        }
    } 

    file.close();

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

void NeuralNetwork::computeCost(Matrix y)
{
    cost = 0;
    Matrix yhat = get_output();
    if(costfunc == CrossEntropy)
    {
        for(unsigned i=0; i < y[0].size(); i++)
        {
            cost += ( y[0][i] * log2( yhat[0][i] ) );
        }
        cost *= -1;
    }
    cout << "Cost: " << cost << endl;
}

void NeuralNetwork::computeCost(vector<float> y)
{
    Matrix m(1, y.size());
    m[0] = y;
    computeCost(m);
}

void NeuralNetwork::updateWeights(Matrix y)
{//@TODO
    Matrix yhat = get_output();
}

