#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <functional>


using std::vector;

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
            for( unsigned i=0; i < rows; i++ )
            {
                vector<float> temp(cols);
                generate(temp.begin(), temp.end(), [](){return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ;}); // random values
                m_matrix.push_back(temp);
            }
        }

        void print(int rows, int cols);

        Matrix tranpose();

        Matrix apply( std::function<float (float)> activation  );

        vector<float> operator[](size_t i);
        Matrix operator+(Matrix m);
        Matrix operator*(Matrix m);
        void operator+=(Matrix m);

};

#endif