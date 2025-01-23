//
//  Matrix3.c
//  CPhoton
//
//  Created by Edward on 23/01/2023.
//

#include "Matrix3.h"

typedef double Matrix3[3][3];

struct rotate3_t
{
    Matrix3 rotate;
    Matrix3 inverse;
};


static inline Vector3 transformVector(Vector3 v, Matrix3 mat);
static inline void setRotationMatrices(Matrix3 matRot, Matrix3 matInvRot, Vector3 rotAngles);
static inline void rotationMatrix(Matrix3 matRot, double alpha, double beta, double gamma);
static inline void transposeMatrix(Matrix3 matT, Matrix3 mat);


#pragma mark - *** User Accessible ***

Rotate3 *makeRotate3(Vector3 rotationAngles)
{
    if (isNearlyZero(rotationAngles))
        return NULL; // No rotation matrix required.

    Rotate3 *rotation = malloc(sizeof(Rotate3));
    if (!rotation)
        return NULL;

    setRotationMatrices(rotation->rotate, rotation->inverse, rotationAngles);
    return rotation;
}


Vector3 rotation(Vector3 v, Rotate3 *rotation)
{
    if (!rotation)
        return v;

    return transformVector(v, rotation->rotate);
}


Vector3 inverseRotation(Vector3 v, Rotate3 *rotation)
{
    if (!rotation)
        return v;

    return transformVector(v, rotation->inverse);
}


#pragma mark - *** Private Functions ***

static inline void transposeMatrix(Matrix3 matT, Matrix3 mat)
{
    for (int iRow = 0; iRow < 3; iRow++)
    {
        for (int iCol = 0; iCol < 3; iCol++)
        {
            matT[iCol][iRow] = mat[iRow][iCol];
        }
    }
}


static inline void rotationMatrix(Matrix3 matRot, double alpha, double beta, double gamma)
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


static inline Vector3 transformVector(Vector3 v, Matrix3 mat)
{
    const double xPrime = v.x * mat[0][0] + v.y * mat[0][1] + v.z * mat[0][2];
    const double yPrime = v.x * mat[1][0] + v.y * mat[1][1] + v.z * mat[1][2];
    const double zPrime = v.x * mat[2][0] + v.y * mat[2][1] + v.z * mat[2][2];

    return vector3(xPrime, yPrime, zPrime);
}


/// Set the rotation and inverse rotation matrices using a rotation vector
/// which consists of the rotation angles in degrees (angle x, angle y, angle z).
static inline void setRotationMatrices(Matrix3 matRot, Matrix3 matInvRot, Vector3 rotAngles)
{
    rotationMatrix(matRot, rotAngles.x, rotAngles.y, rotAngles.z);
    transposeMatrix(matInvRot, matRot);
}
