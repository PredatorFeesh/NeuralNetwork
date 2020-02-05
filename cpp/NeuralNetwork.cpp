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

        // b.push_back( Matrix(1, top[i+1]) );
        // w.push_back( Matrix(top[i], top[i+1]) );
        w.push_back( Matrix(top[i+1], top[i]) );
        b.push_back( Matrix(top[i+1], 1) );
        cout << "Making weight: " << top[i+1] << "x" << top[i] << endl;

        // Instantiate Weights

        switch(initmet)
        {
            case Zero: w[i].initZero(); break;
            case LowRandoms: w[i].initLowRandoms(); break;
            case Xavier: w[i].initXavier(top[i], top[i+1]); break;
            case Kaiming: w[i].initKaming(); break;
        }

        // nab_b.push_back( Matrix(1, top[i+1]) );
        // nab_w.push_back( Matrix(top[i], top[i+1]) );
        nab_w.push_back( Matrix(top[i+1], top[i]) );
        nab_b.push_back( Matrix(top[i+1], 1) );
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
    // cout << "GOT DATA IN FORWARD PROP: " << endl;
    // X.print(10, 1);
    // This is for input layer
    z[0] = X;
    a[0] = X;

    cout << "a[0] :: " << a[0].rows << "x" << a[0].cols << endl;
    a[0].print(10 , a[0].cols);


    // This is for hidden layers
    for(unsigned i = 1; i < net_size; i++)
    {
        // cout << "On layer " << i << endl;
        z[i] = w[i-1]*a[i-1];
        z[i] += b[i-1];
        if (activfunc == SIGMOID)
            a[i] = z[i].apply(maths::sigmoid);

        // cout << "z["<<i<<"]" << endl;
        // z[i].print(10,1);
        // cout << "a["<<i<<"]" << endl;
        // a[i].print(10,1);
    }
    cout << "Final Layer " << endl;

    // This is for the final layer
    z[net_size] = w[net_size-1] * a[net_size-1];
    z[net_size] += b[net_size-1];

    if ( finalfunc == SOFTMAX )
    {
        a[net_size] = z[net_size].apply(maths::expon) / maths::expsum( z[net_size].transpose().m_matrix[0] );
    }
    else if ( finalfunc == SIGMOIDAL )
    {
        a[net_size] = z[net_size].apply(maths::sigmoid);
    }

    cout << "a[1]" << a[1].rows << "x" << a[1].cols << endl;
    a[1].print(10,1);
    cout << "Final Layer Output" << a[net_size].rows << "x" << a[net_size].cols << endl;
    a[net_size].print(10,1);

}

void NeuralNetwork::computeCost(Matrix y)
{
    cost = 0;
    Matrix yhat = get_output();

    cout << "Correct choice: " << endl;
    y.print(y.rows, y.cols);
    cout << "Gotten choice: " << endl;
    yhat.print(yhat.rows, yhat.cols);

    if(costfunc == CrossEntropy)
    {
        for(unsigned i=0; i < y.rows; i++)
        {
            cost += ( y[i][0] * log2( yhat[i][0] ) );
        }
        cost *= -1;
    }

    m_recentAverageError = (m_recentAverageError * m_smoothingFactor + cost) / (m_recentAverageError + 1.0f);

    // cout << endl;
    // cout << "Cost: " << cost << endl;
    // cout << "Average Error: " << m_recentAverageError << endl;
    // cout << endl;
    if (cost != cost)
    {
        cout << "Got cost of NAN!" << endl;
        saveNetwork("debug.txt");
        exit(1);
    }
}



void NeuralNetwork::updateWeights(Matrix y, float learn_rate)
{//@TODO
    Matrix yhat = get_output();

    // Loss deriv
    // Matrix dl( 1, a[net_size][1].size() );

    Matrix dcost(0,0);
    // Last Layer
    // cout << "Starting last layer backprop" << endl;
    if(costfunc == CrossEntropy && finalfunc == SOFTMAX) // Just for now, since this is common and project needs to get sped up. Fixed in later branch
    {
        // dl = yhat.apply( maths::inverse );
        dcost = (a[net_size] - y); // 1x10
    }

    nab_w[net_size-1] = a[net_size-1] * dcost.transpose()  ;
    nab_b[net_size-1] = dcost;

    // cout << "Updating weight" << endl;
    // Update weights
    w[net_size-1] -= (nab_w[net_size-1] * learn_rate).transpose();
    b[net_size-1] -= nab_b[net_size-1];

    // cout << "Starting to update hidden" << endl;
    // Hidden Layers
    for( int i = net_size-2; i >= 0; i-- )
    {
        cout << "On hidden layer " << i << endl;
        if(activfunc == SIGMOID)
        {
            cout << "Z: " << z[i+1].rows << "x" << z[i+1].cols << endl;
            cout << "Dcost: " << dcost.rows << "x" << dcost.cols << endl;
            cout << "W: " << w[i+1].rows << "x" << w[i+2].cols << endl;
            dcost = z[i+1].apply(maths::sigmoid_prime).hadmard( w[i+1].transpose() * dcost );
        }
        cout << "Computed dcost" << endl;
        nab_w[i] = dcost * a[i].transpose();
        nab_b[i] = dcost;

        cout << "Updated deltas" << endl;

        // Update weights
        w[i] -= nab_w[i] * learn_rate;
        b[i] -= nab_b[i];
        cout << "Updated weights" << endl;
    }

}

void NeuralNetwork::train(Cifar::Cifar data, unsigned int num_iters, float learn_rate)
{
    for(unsigned i=0; i<num_iters; i++)
    {
        auto batch = data.get_random_train();

        Matrix X = Matrix(batch[0]);
        Matrix y = Matrix(batch[1]);

        cout << "DATA RECIEVED FROM CIFAR " << endl;
        X.print(10,1);
        y.print(10,1);

        forward(X);
        computeCost(y);
        updateWeights( y , learn_rate );

        cout << endl;
        cout << "COST: " << cost << endl;
        cout << "AVERAGE ERROR: " << m_recentAverageError << endl;
        cout << endl;

        if( i == 100 )
        {
            cout << "REACHED THE " << i << "TH ITER" << endl;
            saveNetwork("debug.txt");
            exit(1);
        }

    }
}

