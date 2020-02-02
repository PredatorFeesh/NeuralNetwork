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


enum InitType
{
    ROWS,
    COLS
};

class Matrix
{
    public:
        vector<vector<float>> m_matrix;
        size_t rows;
        size_t cols;

        Matrix(int rows, int cols) : rows(rows), cols(cols)
        {
            m_matrix.reserve(rows);

            for( unsigned i=0; i < rows; i++ )
            {
                vector<float> temp(cols, 0.0f);
                m_matrix.push_back(temp);
            }

        }

        Matrix(vector<float> vec, InitType init = ROWS )
        {
            if(init == ROWS)
            {
                rows = vec.size();
                cols = 1;
                m_matrix.reserve(rows);
                for(unsigned i = 0; i < rows; i++)
                    m_matrix.push_back(vector<float>{vec[i]});

            } else if (init == COLS)
            {
                rows = 1;
                cols = vec.size();
                m_matrix.push_back(vec);
            }
        }

        void initLowRandoms(float bot = -0.01f, float top = 0.01f );
        void initZero();
        void initXavier(float n_i1, float n_i2);
        void initKaming(){cout << "NOT IMPLEMENTED YET! KAIMING!" << endl; exit(1); };

        void print(int rows, int cols);

        Matrix transpose();

        Matrix apply( std::function<float (float)> activation  );
        Matrix apply( std::function<float (float, float)> activation, float param );

        vector<float> operator [](size_t i) const
        {
            if ( i >= rows || i < 0 ) { cout << "Index out of range! Index: " << i << endl; exit(1); }
            return m_matrix[i];
        }
        vector<float> & operator [](size_t i)
        {
            if ( i >= rows || i < 0 ) { cout << "Index out of range! Index: " << i << endl; exit(1); }
            return m_matrix[i];
        }

        Matrix operator+(Matrix m);
        Matrix operator-(Matrix m);
        Matrix operator*(Matrix m);
        Matrix operator*(float x);
        Matrix hadmard(Matrix m); // Hadamard Product
        Matrix operator/(float a);
        void operator+=(Matrix m);
        void operator-=(Matrix m);

        void operator=(Matrix m); // Have to override; otherwise throws error

};

#endif