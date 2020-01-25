#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#pragma once

#include <math.h>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <numeric>

namespace maths
{

    inline float sigmoid(float x)
    {
        return 1.0f / (1.0f + exp(-x ) );
    }

    inline float sigmoid_prime(float x)
    {
        return ( x * (1.0f - x)  );
    }

    inline float relu(float x)
    {
        return (x <= 0) ? 0 : x;
    }

    inline float expsum(vector<float> x)
    {
        // @TODO: TEST
        float sum = 0;
        for_each(x.begin(), x.end(), [&sum](float n){
            sum += std::exp(n);
        });
        return sum;
    }

    inline float expon(float x)
    {
        return std::exp(x);
    }

    inline float inverse(float x)
    {
        return 1.0f / x;
    }

    inline float softmax_prime(int i, int j, const Matrix p) // from j, to i
    { // This matrix is 1 x layer
        return (i == j) ? p[0][i] * p[0][j] : p[0][i] * (1 - p[0][j]);
    }


};



#endif