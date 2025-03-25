/**
 * @file Vector3.cpp
 * @author Edward Palmer
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Vector3.hpp"
#include <cmath>

extern "C"
{
#include "Randomizer.h"
}

Vector3::Vector3(double x, double y, double z) : _x(x), _y(y), _z(z)
{
}


double Vector3::lengthSquared() const
{
    return (_x * _x + _y * _y + _z * _z);
}


double Vector3::length() const
{
    return std::sqrt(lengthSquared());
}


Vector3 Vector3::randomVector(double min = 0.0, double max = 1.0)
{
    return Vector3(randomDoubleRange(min, max), randomDoubleRange(min, max), randomDoubleRange(min, max));
}


Vector3 Vector3::randomUnitSphereVector()
{
    Vector3 vectorOut;
    double vectorLengthSquared;

    do
    {
        vectorOut = randomVector(-1.0, 1.0);
        vectorLengthSquared = vectorOut.lengthSquared();
    } while (vectorLengthSquared >= 1.0);

    return vectorOut;
}


Vector3 Vector3::randomUnitVector()
{
    return randomUnitSphereVector().normalize();
}


Vector3 Vector3::randomUnitDiskVector()
{
    Vector3 vectorOut;
    double vectorLengthSquared;

    do
    {
        vectorOut = Vector3(randomDoubleRange(-1, 1), randomDoubleRange(-1, 1), 0);
        vectorLengthSquared = vectorOut.lengthSquared();
    } while (vectorLengthSquared >= 1.0);

    return vectorOut;
}


Vector3 Vector3::normalize() const
{
    double vLength = length();

    // TODO: - this is slightly weird logic.
    return (vLength > 0.0) ? (*this) * (1.0 / vLength) : (*this);
}


double Vector3::dot(const Vector3 &other) const
{
    return (x() * other.x() + y() * other.y() + z() * other.z());
}


Vector3 Vector3::cross(const Vector3 &other) const
{
    double iComponent = y() * other.z() - z() * other.y();
    double jComponent = z() * other.x() - x() * other.z();
    double kComponent = x() * other.y() - y() * other.x();

    return Vector3(iComponent, jComponent, kComponent);
}


Vector3 &Vector3::operator-()
{
    _x = -_x;
    _y = -_y;
    _z = -_z;

    return (*this);
}


Vector3 Vector3::operator*(double scalar) const
{
    return Vector3(x() * scalar, y() * scalar, z() * scalar);
}


Vector3 &Vector3::operator*=(double scalar)
{
    _x *= scalar;
    _y *= scalar;
    _z *= scalar;

    return (*this);
}


bool Vector3::isApproxZero(double tolerance = 1.0e-8) const
{
    return (std::fabs(_x) < tolerance && std::fabs(_y) < tolerance && std::fabs(_z) < tolerance);
}


Vector3 Vector3::operator+(const Vector3 &other) const
{
    return Vector3(x() + other.x(), y() + other.y(), z() + other.z());
}


Vector3 &Vector3::operator+=(const Vector3 &other)
{
    _x += other.x();
    _y += other.y();
    _z += other.z();

    return (*this);
}


Vector3 Vector3::operator-(const Vector3 &other) const
{
    return Vector3(x() - other.x(), y() - other.y(), z() - other.z());
}


Vector3 &Vector3::operator-=(const Vector3 &other)
{
    _x -= other.x();
    _y -= other.y();
    _z -= other.z();

    return (*this);
}