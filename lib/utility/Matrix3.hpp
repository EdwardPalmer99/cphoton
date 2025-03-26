/**
 * @file Matrix3.hpp
 * @author Edward Palmer
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Vector3.hpp"

class Rotate3
{
public:
    /* Constructors. NB: angles are in degrees */
    Rotate3() = delete;
    Rotate3(const Vector3 &angles);

    /* Applies a rotation to a vector */
    Vector3 rotate(const Vector3 &vector) const;

    /* Undoes a rotation */
    Vector3 undoRotation(const Vector3 &vector) const;

    /* Returns true if it is a zero rotation */
    constexpr bool isZeroRotation() const
    {
        return _isZeroRotation;
    }

protected:
    using Matrix3 = double[3][3];

    /* Sets a rotation matrix */
    void rotationMatrix(Matrix3 &matRot, double alpha, double beta, double gamma);

    /* Transposes a matrix */
    void transposeMatrix(Matrix3 &matT, Matrix3 &mat);

    /* Applies transformation to a vector */
    Vector3 transformVector(const Vector3 &v, const Matrix3 &mat) const;

    /* If the rotation angle is zero then don't apply */
    bool _isZeroRotation{false};

    Matrix3 _rotation;
    Matrix3 _inverse;
};
