#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <functional>
#include <random>

using std::vector;

enum MatrixInit
{
    
};

class Matrix
{
    public:
        vector<vector<float>> m_matrix;
        size_t rows;
        size_t cols;

    // public:
        Matrix(int rows, int cols) : rows(rows), cols(cols)
        {
            m_matrix.reserve(rows);

            // std::default_random_engine generator;
            // std::uniform_real_distribution<double> distribution(0.0,0.001);

            // for( unsigned i=0; i < rows; i++ )
            // {
            //     vector<float> temp(cols);
            //     generate(temp.begin(), temp.end(), [&](){return distribution(generator) ;}); // random values
            //     m_matrix.push_back(temp);
            // }

            for( unsigned i=0; i < rows; i++ )
            {
                vector<float> temp(cols, 0.0f);
                m_matrix.push_back(temp);
            }

        }

        void print(int rows, int cols);

        Matrix tranpose();

        Matrix apply( std::function<float (float)> activation  );
        Matrix apply( std::function<float (float, float)> activation, float param );

        vector<float> operator [](size_t i) const {return m_matrix[i];}
        vector<float> & operator [](size_t i) {return m_matrix[i];}

        Matrix operator+(Matrix m);
        Matrix operator*(Matrix m);
        Matrix operator/(float a);
        void operator+=(Matrix m);

};

#endif