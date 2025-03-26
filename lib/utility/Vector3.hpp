/**
 * @file Vector3.hpp
 * @author Edward Palmer
 * @date 2025-03-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

class Vector3
{
public:
    /* Zero vector by default */
    Vector3() = default;

    /* Constructor */
    explicit Vector3(double x, double y, double z);

    /* Const-accessors */
    constexpr double x() const
    {
        return _x;
    }

    constexpr double y() const
    {
        return _y;
    }

    constexpr double z() const
    {
        return _z;
    }

    /* Accessors */
    inline double &x()
    {
        return _x;
    }

    inline double &y()
    {
        return _y;
    }

    inline double &z()
    {
        return _z;
    }

    /* Returns a random vector */
    static Vector3 randomVector(double min = 0.0, double max = 1.0);

    /*
     * Returns a random vector inside a unit sphere. Points are generated in the range [-1, 1] for x, y, z. If the
     * length squared is less than or equal to 1.0 then it is inside the unit sphere.
     */
    static Vector3 randomUnitSphereVector();

    /*
     * Generate a vector inside the unit sphere then normalize
     */
    static Vector3 randomUnitVector();

    /* Returns a vector inside a unit disk */
    static Vector3 randomUnitDiskVector();

    /* Returns true if vector is close to zero */
    bool isApproxZero(double tolerance = 1.0e-8) const;

    /* Returns the length of the vector squared */
    double lengthSquared() const;

    /* Returns the length of the vector */
    double length() const;

    /* Returns a normalized vector (unit vector) */
    Vector3 normalize() const;

    /* Dot operation */
    double dot(const Vector3 &other) const;

    /* Cross operation */
    Vector3 cross(const Vector3 &other) const;

    /* Negation operator (i.e. flip a vector) */
    Vector3 &operator-();

    /* Scale operator */
    Vector3 operator*(double scalar) const;

    /* Vector scale operation */
    Vector3 operator*(const Vector3 &other) const;

    /* Multiplication assignment operator */
    Vector3 &operator*=(double scalar);

    /* Addition operator */
    Vector3 operator+(const Vector3 &other) const;

    /* Addition assignment operator */
    Vector3 &operator+=(const Vector3 &other);

    /* Subtraction operation */
    Vector3 operator-(const Vector3 &other) const;

    /* Subtraction assigment operator */
    Vector3 &operator-=(const Vector3 &other);

protected:
    double _x{0.0};
    double _y{0.0};
    double _z{0.0};
};

/* Aliases */
using Point3 = Vector3;
using Color3 = Vector3;
