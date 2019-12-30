#ifndef MATH_HELPER_H
#define MATH_HELPER_H
#pragma once

#include <math.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

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

};



#endif