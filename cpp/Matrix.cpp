#include "../headers/Matrix.h"
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

void Matrix::print(int rows, int cols)
{
    for( unsigned i = 0; i < rows; i++ )
    {
        for ( unsigned j = 0; j < cols; j++)
            cout << m_matrix[i][j] << " ";
        cout << ";" << endl;
    }
}

Matrix Matrix::tranpose()
{
    Matrix output(cols, rows);
    for( unsigned i = 0; i < rows; i++ )
        for( unsigned j = 0; j < cols; j++ )
            output[j][i] = m_matrix[i][j];
    return output;
}

Matrix Matrix::apply( std::function<float (float)> activation )
{
    Matrix output(rows, cols);
    for( unsigned i = 0; i < rows; i++ )
        for( unsigned j = 0; j < cols; j++ )
            output[i][j] = activation(m_matrix[i][j]);
    return output;
}



Matrix Matrix::operator+(Matrix m)
{
    cout << "Adding" << endl;
    cout << rows << " " << cols << endl;
    cout << m.rows << " " << m.cols << endl;
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

vector<float> Matrix::operator[](size_t i)
{
    return m_matrix[i];
}

Matrix Matrix::operator*(Matrix m)
{
    cout << "Multiplying" << endl;
    cout << rows << " " << cols << endl;
    cout << m.rows << " " << m.cols << endl;
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