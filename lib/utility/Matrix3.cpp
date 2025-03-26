/**
 * @file Matrix3.cpp
 * @author Edward Palmer
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Matrix3.hpp"
#include <cmath>

extern "C"
{
#include "utility/MathMacros.h"
}

Rotate3::Rotate3(const Vector3 &angles) : _isZeroRotation(angles.isApproxZero())
{
    if (!_isZeroRotation)
    {
        /* Prepare matrices */
        rotationMatrix(_rotation, angles.x(), angles.y(), angles.z());
        transposeMatrix(_inverse, _rotation);
    }
}


Vector3 Rotate3::rotate(const Vector3 &vector) const
{
    if (_isZeroRotation)
    {
        return vector;
    }

    return transformVector(vector, _rotation);
}


Vector3 Rotate3::undoRotation(const Vector3 &vector) const
{
    if (_isZeroRotation)
    {
        return vector;
    }

    return transformVector(vector, _inverse);
}


void Rotate3::rotationMatrix(Matrix3 &matRot, double alpha, double beta, double gamma)
{
    double cosAlpha = cosd(alpha), cosBeta = cosd(beta), cosGamma = cosd(gamma);
    double sinAlpha = sind(alpha), sinBeta = sind(beta), sinGamma = sind(gamma);

    matRot[0][0] = cosBeta * cosGamma;
    matRot[0][1] = sinAlpha * sinBeta * cosGamma - cosAlpha * sinGamma;
    matRot[0][2] = cosAlpha * sinBeta * cosGamma + sinAlpha * sinGamma;

    matRot[1][0] = cosBeta * sinGamma;
    matRot[1][1] = sinAlpha * sinBeta * sinGamma + cosAlpha * cosGamma;
    matRot[1][2] = cosAlpha * sinBeta * sinGamma - sinAlpha * cosGamma;

    matRot[2][0] = -sinBeta;
    matRot[2][1] = sinAlpha * cosBeta;
    matRot[2][2] = cosAlpha * cosBeta;
}


void Rotate3::transposeMatrix(Matrix3 &matT, Matrix3 &mat)
{
    for (int iRow = 0; iRow < 3; iRow++)
    {
        for (int iCol = 0; iCol < 3; iCol++)
        {
            matT[iCol][iRow] = mat[iRow][iCol];
        }
    }
}


Vector3 Rotate3::transformVector(const Vector3 &v, const Matrix3 &mat) const
{
    const double xPrime = v.x() * mat[0][0] + v.y() * mat[0][1] + v.z() * mat[0][2];
    const double yPrime = v.x() * mat[1][0] + v.y() * mat[1][1] + v.z() * mat[1][2];
    const double zPrime = v.x() * mat[2][0] + v.y() * mat[2][1] + v.z() * mat[2][2];

    return Vector3(xPrime, yPrime, zPrime);
}