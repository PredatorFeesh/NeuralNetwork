#include "../headers/Matrix.h"
#include <iostream>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::transform;

void Matrix::initZero()
{
    // Done by default so pass
}

void Matrix::initLowRandoms(float low, float top)
{
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dist(low, top);


    for( unsigned i=0; i < rows; i++ )
    {
        generate(m_matrix[i].begin(), m_matrix[i].end(), [&](){return dist(gen) ;}); // random values
    }

}

void Matrix::initXavier(float n_i1, float n_i2) // n_i1 is layer i, n_i2 is layer i+1
{
    float sqrt6 = sqrt(6);
    initLowRandoms( - (sqrt6 / ( n_i1 + n_i2 ) ) , (sqrt6 / ( n_i1 + n_i2 ) ) );
}

void Matrix::print(int rows, int cols)
{
    for( unsigned i = 0; i < rows; i++ )
    {
        for ( unsigned j = 0; j < cols; j++)
            cout << m_matrix[i][j] << " ";
        cout << ";" << endl;
    }
}

Matrix Matrix::transpose()
{
    Matrix output(cols, rows);
    for( unsigned i = 0; i < rows; i++ )
        for( unsigned j = 0; j < cols; j++ )
            output.m_matrix[j][i] = m_matrix[i][j];
    return output;
}

Matrix Matrix::apply( std::function<float (float)> activation )
{
    Matrix output(rows, cols);
    for( unsigned i = 0; i < rows; i++ )
        for( unsigned j = 0; j < cols; j++ )
            output[i][j] = activation( m_matrix[i][j] );
    return output;
}

Matrix Matrix::operator/(float a)
{
    Matrix out(rows, cols);

    for(unsigned i=0; i<rows; i++)
        for(unsigned j=0; j<cols; j++)
            out.m_matrix[i][j] = m_matrix[i][j]/a;

    return out;

}

Matrix Matrix::operator+(Matrix m)
{
    // cout << "Adding" << endl;
    // cout << rows << " " << cols << endl;
    // cout << m.rows << " " << m.cols << endl;
    if ( m.rows != rows || m.cols != cols ){ cout << "Parameters don't match! Can't add!"; exit(1); }

    Matrix out(rows, cols);

    int i = 0;

    for( i = 0; i < rows; i++ )
    {
        // for( j = 0; j < cols; j++)
        // {
        //     output.m_matrix[i][j] = m_matrix[i][j] + m.m_matrix[i][j];
        //     cout << output[i][j];
        // }
        std::transform( m_matrix[i].begin(), m_matrix[i].end(), m.m_matrix[i].begin(), out.m_matrix[i].begin(), std::plus<float>() );
    }
    cout << "Done adding!" << endl;

    return out; // We get an error on return? Why?
}


Matrix Matrix::operator*(Matrix m)
{
    // cout << "Multiplying" << endl;
    // cout << rows << " " << cols << endl;
    // cout << m.rows << " " << m.cols << endl;
    if ( cols != m.rows ){ cout << "Rows != Col! Can't multiply!"; exit(1); }

    Matrix output(rows, m.cols);

    int i, j, k;
	// Initializing elements of matrix mult to 0.
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < m.cols; ++j)
		{
			output[i][j] = 0;
		}
	}
	// Multiplying matrix firstMatrix and secondMatrix and storing in array mult.
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < m.cols; ++j)
		{
			for(k=0; k<cols; ++k)
			{
				output[i][j] += m_matrix[i][k] * m.m_matrix[k][j];
			}
		}
	}

    return output;

}

void Matrix::operator+=(Matrix m)
{
    *this = *this + m;
}
void Matrix::operator-=(Matrix m)
{
    *this = *this - m;
}

Matrix Matrix::operator-(Matrix m)
{
    if ( rows != m.rows || cols != m.cols){ cout << "Cant sub! Rows and Cols noteq :: " << rows<<"x"<<cols<<" - "<<m.rows<<"x"<<m.cols<< endl; exit(1); }
    Matrix temp(rows, cols);
    return *this + (m * -1);
}

Matrix Matrix::operator*(float x)
{
    Matrix temp(rows, cols);
    for(unsigned i = 0; i < rows; i++)
        for(unsigned j = 0; j < cols; j++)
            temp[i][j] = m_matrix[i][j] * x;
    return temp;
}
Matrix Matrix::hadmard(Matrix m)
{
    if ( rows != m.rows || cols != m.cols ) { cout << "Hadmard error! : " << rows<<"x"<<cols<<" = "<<m.rows<<"x"<<m.cols << endl; exit(1); }
    Matrix temp(rows, cols);
    for(unsigned i = 0; i < rows; i++)
        for(unsigned j = 0; j < cols; j++)
            temp[i][j] = m_matrix[i][j] * m.m_matrix[i][j];
    return temp;
}

void Matrix::operator=(Matrix m)
{
    // if (rows != m.rows || cols != m.cols) {cout << "Cant equate! Rows and Cols noteq :: " << rows<<"x"<<cols<<" = "<<m.rows<<"x"<<m.cols<< endl; exit(1);};
    rows = m.rows;
    cols = m.cols;
    m_matrix = m.m_matrix;
    // m_matrix.reserve(rows);
    // for(unsigned i = 0; i < rows; i++)
    // {
    //     // m_matrix[i].reserve(cols);
    //     for(unsigned j = 0; j < cols; j++)
    //         {
    //             m_matrix[i][j] = m.m_matrix[i][j];
    //         }
    // }
}