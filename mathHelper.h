#include <math.h>

template <typename type>
type sigmoid(type x, float degrees = 1)
{
    return (type)1 / ((type)1 + exp(-x) );
}

template <typename type>
type sigmoid_prime(type x, float degrees = 1)
{
    return ( (exp(-x))/( ( (type)1 + exp(-x) ) * (type)1 + exp(-x) ));
}



