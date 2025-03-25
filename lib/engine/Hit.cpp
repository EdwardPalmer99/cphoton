/**
 * @file Hit.cpp
 * @author Edward Palmer
 * @date 2025-03-24
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Hit.hpp"


bool Hit::isValid(Time min, Time max) const
{
    return isValid(t, min, max);
}
