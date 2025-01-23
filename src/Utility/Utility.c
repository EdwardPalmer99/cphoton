//
//  Utility.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "Utility.h"

/// Returns a double in the range [0, 1).
double randomDouble(void)
{
    return (double)rand() / (RAND_MAX + 1.0);
}


/// Returns a double in the range [min, max).
double randomDoubleRange(double min, double max)
{
    return min + randomDouble() * (max - min);
}
