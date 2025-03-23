/**
 * @file RenderSettings.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <cstdint>

/* Singleton storing global render settings. */
class RenderSettings
{
public:
    /* Returns a reference to the config instance. */
    static RenderSettings &instance();

    /* Sets width, height. */
    void setDefaultWidthHeight(uint16_t pixelsWide, uint16_t pixelsHigh);

    /* Returns the aspect ratio. */
    constexpr double aspectRatio() const
    {
        return double(pixelsWide) / double(pixelsHigh);
    }

    /* Member variables */
    uint16_t pixelsWide{0};
    uint16_t pixelsHigh{0};
    char *outputPath{nullptr};

protected:
    /* Protect default constructor */
    RenderSettings() = default;

    /* Delete copy constructors */
    RenderSettings(const RenderSettings &) = delete;
    RenderSettings &operator=(const RenderSettings &) = delete;
};