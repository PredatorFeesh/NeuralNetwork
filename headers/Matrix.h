#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <functional>
#include <random>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

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

            for( unsigned i=0; i < rows; i++ )
            {
                vector<float> temp(cols, 0.0f);
                m_matrix.push_back(temp);
            }

        }

        void initLowRandoms(float bot = -0.01f, float top = 0.01f );
        void initZero();
        void initXavier(float n_i1, float n_i2);
        void initKaming(){cout << "NOT IMPLEMENTED YET! KAIMING!" << endl; exit(1); };

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