/**
 * @file TestLogger.cpp
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gtest/gtest.h>

extern "C"
{
#include "engine/Camera.h"
#include "utility/Vector3.h"
}

class TestCamera : public testing::Test
{
protected:
    void ExpectPointEqual(Point3 point1, Point3 point2)
    {
        EXPECT_DOUBLE_EQ(point1.x, point2.x);
        EXPECT_DOUBLE_EQ(point1.y, point2.y);
        EXPECT_DOUBLE_EQ(point1.z, point2.z);
    }
};


TEST_F(TestCamera, TestMakeCamera)
{
    double vFOV = 45.0;
    double aspectRatio = 1.0;
    double focalLength = 1;
    double aperture = 1;

    Point3 origin = point3(0, 0, 0);
    Point3 target = point3(1, 1, 1);

    Camera camera = makeCamera(vFOV, aspectRatio, focalLength, aperture, origin, target);

    ExpectPointEqual(camera.origin, origin);
    EXPECT_DOUBLE_EQ(camera.lensRadius, aperture / 2.0);
}