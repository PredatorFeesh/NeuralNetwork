#include "headers/Matrix.h"
#include <iostream>

using std::cout;
using std::endl;

int main()
{
    Matrix m1(2,2);
    Matrix m2(2,2);
    Matrix m3(2,3);

    m1[0][0] = 1;
    m1[0][1] = 2;
    m1[1][0] = 3;
    m1[1][1] = 4;
    cout << "Initiated m1" << endl;
    m1.print(m1.rows, m1.cols);

    m2[0][0] = 2;
    m2[0][1] = 1;
    m2[1][0] = 4;
    m2[1][1] = 3;
    cout << "Initiated m2" << endl;
    m2.print(m2.rows, m2.cols);

    m3[0][0] = 3;
    m3[0][1] = 4;
    m3[0][2] = 1;
    m3[1][0] = 2;
    m3[1][1] = 1;
    m3[1][2] = 2;
    cout << "Initiated m3" << endl;
    m3.print(m3.rows, m3.cols);

    cout << "Testing Matrix Multiplication" << endl;
    Matrix test = m1 * m2;
    if( test[0][0] == 10  && test[0][1] == 7  && test[1][0] == 22  && test[1][1] == 15  )
    {
        cout << "Matrix Mult test 1 pass!" << endl;
    } else {
        cout << "Matrix Mult test 1 failed!" << endl;
    }

    cout << "Testing Transpose" << endl;
    test = m1.transpose();
    if( test[0][0] == 1  && test[0][1] == 3  && test[1][0] == 2  && test[1][1] == 4  )
    {
        cout << "Matrix Trans test 1 pass!" << endl;
    } else {
        cout << "Matrix Trans test 1 failed!" << endl;
    }

    cout << "Testing hadmard" << endl;
    test = m1.hadmard(m2);
    if( test[0][0] == 2  && test[0][1] == 2  && test[1][0] == 12  && test[1][1] == 12  )
    {
        cout << "Matrix Had test 1 pass!" << endl;
    } else {
        cout << "Matrix Had test 1 failed!" << endl;
    }

    cout << "Teting add" << endl;
    test = m1 + m2;
    if( test[0][0] == 3  && test[0][1] == 3  && test[1][0] == 7  && test[1][1] == 7  )
    {
        cout << "Matrix Add test 1 pass!" << endl;
    } else {
        cout << "Matrix Add test 1 failed!" << endl;
    }

    cout << "Teting subtract" << endl;
    test = m1 - m2;
    if( test[0][0] == -1  && test[0][1] == 1  && test[1][0] == -1  && test[1][1] == 1  )
    {
        cout << "Matrix Sub test 1 pass!" << endl;
    } else {
        cout << "Matrix Sub test 1 failed!" << endl;
    }

    cout << "Testing -=" << endl;
    test -= m1;
    if( test[0][0] == -2  && test[0][1] == -1  && test[1][0] == -4  && test[1][1] == -3  )
    {
        cout << "Matrix Sub= test 1 pass!" << endl;
    } else {
        cout << "Matrix Sub= test 1 failed!" << endl;
    }

    return 0;
}