/**
 * @file PPMWriter.cpp
 * @author Edward Palmer
 * @date 2025-03-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "PPMWriter.hpp"
#include <cmath>
#include <cstdint>
#include <functional>
#include <stdexcept>

extern "C"
{
#include "utility/MathMacros.h"
}

#define MAX_BYTE 255
#define MAX_2BYTES 65535

#define MASK_RIGHT_BYTE (MAX_2BYTES - MAX_BYTE)
#define FLIP_BYTES(bytes) (((bytes) >> 8) | (((bytes) << 8) & MASK_RIGHT_BYTE))


PPMWriter::PPMWriter(int pixelsWide, int pixelsHigh) : _pixelsWide(pixelsWide), _pixelsHigh(pixelsHigh)
{
    if (pixelsWide < 1 || pixelsHigh < 1)
    {
        throw std::runtime_error("invalid image dimensions specified");
    }

    _pixels = std::make_unique<Pixel>(numPixels());
}


bool PPMWriter::write(std::string path, FileType type) const
{
    if (path.empty())
    {
        return false;
    }

    /* Open file */
    std::unique_ptr<FILE> fp(fopen(path.c_str(), (type == PlainText8Bit) ? "w" : "wb"));
    if (!fp)
    {
        return false;
    }

    /* Select correct method for writing each pixel */
    decltype(&PPMWriter::writePixel16BitBinary) pixelWriterPtr{nullptr};

    /* Write the header */
    switch (type)
    {
        case PlainText8Bit:
            pixelWriterPtr = writePixel8BitPlainText;
            fprintf(fp.get(), "P3\n%d %d\n255\n", _pixelsWide, _pixelsHigh);
            break;
        case Binary8Bit:
            pixelWriterPtr = writePixel8BitBinary;
            fprintf(fp.get(), "P6\n%d %d\n255\n", _pixelsWide, _pixelsHigh);
            break;
        case Binary16Bit:
        default:
            pixelWriterPtr = writePixel16BitBinary;
            fprintf(fp.get(), "P6\n%d %d\n65535\n", _pixelsWide, _pixelsHigh);
            break;
    }

    auto pixelWriter = std::bind(pixelWriterPtr, this, *fp, std::placeholders::_1);

    /* Write individual pixels */
    for (int i = _pixelsHigh - 1; i >= 0; i--)
    {
        for (int j = 0; j < _pixelsWide; j++)
        {
            pixelWriter(pixel(i, j));
        }
    }

    return true;
}


void PPMWriter::writePixel8BitPlainText(FILE &fp, const Pixel &thePixel) const
{
    // Gamma correct. The pixel input values should be in the range [0, 1].
    Pixel gammaCorrectedPixel = applyGammaCorrection(thePixel);

    uint8_t bytes[3];

    for (int i = 0; i < 3; ++i)
        bytes[i] = apply8BitClamp(gammaCorrectedPixel.arr[i]);

    // Write translated [0, 255] value for each color component.
    fprintf(&fp, "%u %u %u\n", bytes[0], bytes[1], bytes[2]);
}


void PPMWriter::writePixel8BitBinary(FILE &fp, const Pixel &thePixel) const
{
    // Gamma correct. The pixel input values should be in the range [0, 1].
    Pixel gammaCorrectedPixel = applyGammaCorrection(thePixel);

    // Write translated [0, 255] value for each color component.
    uint8_t bytes[3];

    for (int i = 0; i < 3; ++i)
        bytes[i] = apply8BitClamp(gammaCorrectedPixel.arr[i]);

    fwrite(bytes, 1, 3, &fp);
}


void PPMWriter::writePixel16BitBinary(FILE &fp, const Pixel &thePixel) const
{
    // Gamma correct. The pixel input values should be in the range [0, 1].
    Pixel gammaCorrectedPixel = applyGammaCorrection(thePixel);

    // Write translated [0, 65535] value for each color component and flip byte ordering.
    uint16_t bytes[3];

    for (int i = 0; i < 3; ++i)
        bytes[i] = FLIP_BYTES(apply16BitClamp(gammaCorrectedPixel.arr[i]));

    fwrite(bytes, sizeof(uint16_t), 3, &fp);
}


PPMWriter::Pixel PPMWriter::applyGammaCorrection(const Pixel &thePixel) const
{
    // Gamma correct. The pixel input values should be in the range [0, 1].
    Pixel gammaCorrectedPixel;

    gammaCorrectedPixel.r = pow(thePixel.r, inverseGammaCorrection());
    gammaCorrectedPixel.g = pow(thePixel.g, inverseGammaCorrection());
    gammaCorrectedPixel.b = pow(thePixel.b, inverseGammaCorrection());

    return gammaCorrectedPixel;
}


uint8_t PPMWriter::apply8BitClamp(double pixelValue) const
{
    return uint8_t(256 * clamp(pixelValue, 0.0, 0.999));
}


uint16_t PPMWriter::apply16BitClamp(double pixelValue) const
{
    return uint16_t(65536 * clamp(pixelValue, 0.0, 0.999));
}