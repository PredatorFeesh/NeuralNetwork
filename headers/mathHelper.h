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
        return ( (exp(-x))/( ( 1.0f + exp(-x) ) * (1.0f + exp(-x)) )  );
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


};



#endif