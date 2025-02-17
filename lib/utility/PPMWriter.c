//
//  PPMWriter.c
//  CMandelbrot
//
//  Created by Edward on 02/11/2022.
//

#include "utility/PPMWriter.h"
#include "logger/Logger.h"
#include "utility/Utility.h"
#include <stdint.h>

#define MAX_BYTE 255
#define MAX_2BYTES 65535

#define MASK_RIGHT_BYTE (MAX_2BYTES - MAX_BYTE)
#define FLIP_BYTES(bytes) (((bytes) >> 8) | (((bytes) << 8) & MASK_RIGHT_BYTE))

static bool isValidPPMImage(PPMImage *image);
static inline void writePixelToFile(FILE *fp, Color3 *pixel);
static inline void writeBinaryPixelToFile(FILE *fp, Color3 *pixel);
static inline void writeBinary16BitPixelToFile(FILE *fp, Color3 *pixel);


static bool isValidPPMImage(PPMImage *image)
{
    if (!image || !image->pixels || image->width < 1 || image->height < 1)
    {
        return false;
    }

    return true;
}


PPMImage *makePPMImage(int width, int height)
{
    if (width < 1 || height < 1) return NULL;

    PPMImage *newImage = malloc(sizeof(PPMImage));
    if (!newImage) return NULL;

    newImage->width = width;
    newImage->height = height;

    newImage->pixels = malloc(sizeof(Color3) * width * height);
    if (!newImage->pixels)
    {
        free(newImage);
        return NULL;
    }

    newImage->pixelValue = malloc(sizeof(Color3 *) * height);
    if (!newImage->pixelValue)
    {
        free(newImage->pixels);
        free(newImage);
        return NULL;
    }

    for (int i = 0; i < height; i++)
    {
        newImage->pixelValue[i] = (newImage->pixels + i * width);
    }

    return newImage;
}


void freePPMImage(PPMImage *image)
{
    if (image)
    {
        if (image->pixels) free(image->pixels);
        if (image->pixelValue) free(image->pixelValue);

        free(image);
    }
}


void clearImage(PPMImage *image)
{
    if (!isValidPPMImage(image)) return;

    memset(image->pixels, 0, sizeof(Color3) * image->height * image->width);
}


/// A white image will fade to black over successive frames.
void fadeImage(PPMImage *image, double fadeFactor)
{
    if (!image || fadeFactor <= 0.0 || fadeFactor > 1.0) return;

    Color3 *pixel = NULL;

    for (int i = 0; i < NUM_PIXELS(image); i++)
    {
        pixel = image->pixels + i;

        pixel->r *= fadeFactor;
        pixel->g *= fadeFactor;
        pixel->b *= fadeFactor;
    }
}


void copyImage(PPMImage *imageDst, PPMImage *imageSrc)
{
    if (!imageDst || !imageSrc || imageDst->width != imageSrc->width || imageDst->height != imageSrc->height)
    {
        return;
    }

    memcpy(imageDst->pixels, imageSrc->pixels, sizeof(Color3) * NUM_PIXELS(imageSrc));
}


bool writePPMImage(PPMImage *image, const char *fpath)
{
    // 1. Check image structure and fpath.
    if (!isValidPPMImage(image) || !fpath) return false;

    // 2. Open file.
    FILE *fp = fopen(fpath, "w");
    if (!fp)
    {
        fprintf(stderr, "The file could not be opened!\n");
        return false;
    }

    // 3. Write the header.
    fprintf(fp, "P3\n%d %d\n255\n", image->width, image->height);

    // 4. Write the individual pixels.
    for (int i = image->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < image->width; j++)
        {
            writePixelToFile(fp, &image->pixelValue[i][j]);
        }
    }

    // 5. Close the file.
    fclose(fp);

    return true;
}


bool writeBinaryPPMImage(PPMImage *image, const char *fpath)
{
    // 1. Check image structure and fpath.
    if (!isValidPPMImage(image) || !fpath) return false;

    // 2. Open file.
    FILE *fp = fopen(fpath, "wb");
    if (!fp)
    {
        fprintf(stderr, "The file could not be opened!\n");
        return false;
    }

    // 3. Write the header.
    fprintf(fp, "P6\n%d %d\n255\n", image->width, image->height);

    // 4. Write the individual pixels.
    for (int i = image->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < image->width; j++)
        {
            writeBinaryPixelToFile(fp, &image->pixelValue[i][j]);
        }
    }

    // 5. Close the file.
    fclose(fp);

    return true;
}


bool writeBinary16BitPPMImage(PPMImage *image, const char *fpath)
{
    // 1. Check image structure and fpath.
    if (!isValidPPMImage(image) || !fpath) return false;

    // 2. Open file.
    FILE *fp = fopen(fpath, "wb");
    if (!fp)
    {
        Logger(LogLevelError, "Could not write to file '%s'", fpath);
        return false;
    }

    // 3. Write the header.
    fprintf(fp, "P6\n%d %d\n65535\n", image->width, image->height);

    // 4. Write the individual pixels.
    for (int i = image->height - 1; i >= 0; i--)
    {
        for (int j = 0; j < image->width; j++)
        {
            writeBinary16BitPixelToFile(fp, &image->pixelValue[i][j]);
        }
    }

    // 5. Close the file.
    fclose(fp);

    return true;
}


static inline void writePixelToFile(FILE *fp, Color3 *pixel)
{
    if (!fp || !pixel) return;

    // Gamma correct. The pixel input values should be in the range [0, 1].
    static const double invGamma = (1.0 / 2.2);

    double r = pow(pixel->r, invGamma);
    double g = pow(pixel->g, invGamma);
    double b = pow(pixel->b, invGamma);

    // Write translated [0, 255] value for each color component.
    fprintf(fp, "%d %d %d\n", (int)(256 * clamp(r, 0.0, 0.999)), (int)(256 * clamp(g, 0.0, 0.999)),
            (int)(256 * clamp(b, 0.0, 0.999)));
}


static inline void writeBinaryPixelToFile(FILE *fp, Color3 *pixel)
{
    if (!fp || !pixel) return;

    // Gamma correct. The pixel input values should be in the range [0, 1].
    static const double invGamma = (1.0 / 2.2);

    double r = pow(pixel->r, invGamma);
    double g = pow(pixel->g, invGamma);
    double b = pow(pixel->b, invGamma);

    // Write translated [0, 255] value for each color component.
    uint8_t bytes[3];

    bytes[0] = (int)(256 * clamp(r, 0.0, 0.999));
    bytes[1] = (int)(256 * clamp(g, 0.0, 0.999));
    bytes[2] = (int)(256 * clamp(b, 0.0, 0.999));

    fwrite(bytes, 1, 3, fp);
}


static inline void writeBinary16BitPixelToFile(FILE *fp, Color3 *pixel)
{
    if (!fp || !pixel) return;

    // Gamma correct. The pixel input values should be in the range [0, 1].
    static const double invGamma = (1.0 / 2.2);

    double r = pow(pixel->r, invGamma);
    double g = pow(pixel->g, invGamma);
    double b = pow(pixel->b, invGamma);

    // Write translated [0, 65535] value for each color component and flip
    // byte ordering.
    uint16_t bytes[3];

    bytes[0] = FLIP_BYTES((uint16_t)(65536 * clamp(r, 0.0, 0.999)));
    bytes[1] = FLIP_BYTES((uint16_t)(65536 * clamp(g, 0.0, 0.999)));
    bytes[2] = FLIP_BYTES((uint16_t)(65536 * clamp(b, 0.0, 0.999)));

    fwrite(bytes, sizeof(uint16_t), 3, fp);
}
