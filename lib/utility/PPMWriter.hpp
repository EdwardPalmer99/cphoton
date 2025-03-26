/**
 * @file PPMWriter.hpp
 * @author Edward Palmer
 * @date 2025-03-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Vector3.hpp"
#include <cstdio>
#include <memory>

class PPMWriter
{
public:
    PPMWriter() = delete;
    explicit PPMWriter(int pixelsWide, int pixelsHigh);

    struct Pixel
    {
        Pixel() = default;
        Pixel(const Vector3 &vector) : arr{vector.x(), vector.y(), vector.z()}
        {
        }

        union
        {
            struct
            {
                double r;
                double g;
                double b;
            };

            double arr[3];
        };
    };

    /* Returns a const reference to a pixel */
    constexpr const Pixel &pixel(std::size_t row, std::size_t col) const
    {
        return const_cast<const Pixel &>(pixel(row, col));
    }

    /* Returns a non-const reference to a pixel */
    constexpr Pixel &pixel(std::size_t row, std::size_t col)
    {
        return *(_pixels.get() + (row * _pixelsWide) + col);
    }

    /* Returns the image width */
    constexpr int pixelsWide() const
    {
        return _pixelsWide;
    }

    /* Returns the image height */
    constexpr int pixelsHigh() const
    {
        return _pixelsHigh;
    }

    enum FileType
    {
        PlainText8Bit, /* Plain-text P3 file 8-bit */
        Binary8Bit,    /* Binary P6 8-bit */
        Binary16Bit    /* Binary P6 16-bit */
    };

    /* Write to file */
    bool write(std::string path, FileType type = Binary8Bit) const;

protected:
    const int _pixelsWide;
    const int _pixelsHigh;

    /* Gamma-corrects a pixel */
    Pixel applyGammaCorrection(const Pixel &thePixel) const;

    /* Apply 8-bit clamp on pixel */
    uint8_t apply8BitClamp(double pixelValue) const;

    /* Apply 16-bit clamp on pixel */
    uint16_t apply16BitClamp(double pixelValue) const;

    /* Write pixel to plain-text file */
    void writePixel8BitPlainText(FILE &fp, const Pixel &thePixel) const;

    /* Write pixel to binary file (8-bit) */
    void writePixel8BitBinary(FILE &fp, const Pixel &thePixel) const;

    /* Write pixel to binary file (16-bit) */
    void writePixel16BitBinary(FILE &fp, const Pixel &thePixel) const;

    /* Returns number of pixels in image */
    constexpr long numPixels() const
    {
        return _pixelsWide * _pixelsHigh;
    }

    /* Inverse of gamma correction */
    constexpr double inverseGammaCorrection() const
    {
        return (1.0 / 2.2);
    }

    /* An array of pixels */
    std::unique_ptr<Pixel> _pixels{nullptr};
};