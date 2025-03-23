/**
 * @file RenderSettings.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "RenderSettings.hpp"

extern "C"
{
#include <cstring>
}

RenderSettings &RenderSettings::instance()
{
    /* NB: thread-safe */
    static RenderSettings config;

    return config;
}


void RenderSettings::setDefaultWidthHeight(uint16_t pixelsWide_, uint16_t pixelsHigh_)
{
    pixelsWide = pixelsWide_;
    pixelsHigh = pixelsHigh_;
}
