/**
 * MengerCube.hpp
 *
 * Created by Edward on 17/01/2023.
 */

#pragma once
#include "engine/primitives/BVHNode.hpp"
#include "engine/primitives/Primitive.hpp"

extern "C"
{
#include "utility/Vector3.h"
}

Primitive *makeMengerSponge(int8_t n, Point3 center, double sideLength, std::shared_ptr<Material> material);
