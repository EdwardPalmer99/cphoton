//
//  Texture.c
//  CPhoton
//
//  Created by Edward on 29/01/2023.
//

#include "Texture.h"

static inline Texture *makeEmptyTexture(void);

static inline Color3 solidColorValue(Texture *texture, double u, double v, Point3 *hitPt);
static inline Color3 checkerTextureValue(Texture *texture, double u, double v, Point3 *hitPt);
static inline Color3 imageTextureValue(Texture *texture, double u, double v, Point3 *hitPt);

static inline void destructSolidColor(Texture *texture);
static inline void destructChecker(Texture *texture);
static inline void destructImageTexture(Texture *texture);

static inline void incrementRefCounter(Texture *texture);
static inline void decrementRefCounter(Texture *texture);


// *** Textures ***

struct solidColor_t
{
    Color3 color;
};


struct checker_t
{
    Texture *odd;
    Texture *even;
};


struct imageTexture_t
{
    uint8_t *bytes;
    size_t pixelsWide;
    size_t pixelsHigh;
    size_t bytesPerRow;
    size_t bytesPerPixel;
};


/// Returns a partially initialized texture.
static inline Texture *makeEmptyTexture(void)
{
    Texture *emptyTexture = malloc(sizeof(Texture));
    if (!emptyTexture)
        return NULL;

    emptyTexture->refCount = 0;
    emptyTexture->incrementRefCounter = incrementRefCounter;
    emptyTexture->decrementRefCounter = decrementRefCounter;

    emptyTexture->value = NULL;
    emptyTexture->destructor = NULL;

    return emptyTexture;
}


Texture *makeSolidTexture(Color3 color)
{
    Texture *texture = makeEmptyTexture();
    if (!texture)
        return NULL;

    texture->solidColor = malloc(sizeof(SolidColor));
    if (!texture->solidColor)
    {
        free(texture);
        return NULL;
    }

    texture->solidColor->color = color;
    texture->value = solidColorValue;
    texture->destructor = destructSolidColor;

    return texture;
}


Texture *makeCheckerTexture(Texture *even, Texture *odd)
{
    if (!even || !odd)
        return NULL;

    Texture *texture = makeEmptyTexture();
    if (!texture)
        return NULL;

    texture->checker = malloc(sizeof(Checker));
    if (!texture->checker)
    {
        free(texture);
        return NULL;
    }

    texture->checker->even = even;
    texture->checker->odd = odd;

    odd->incrementRefCounter(odd);
    even->incrementRefCounter(even);

    texture->value = checkerTextureValue;
    texture->destructor = destructChecker;

    return texture;
}


Texture *makeCheckerTextureWithColors(Color3 color1, Color3 color2)
{
    return makeCheckerTexture(makeSolidTexture(color1), makeSolidTexture(color2));
}


Texture *makeImageTexture(uint8_t *bytes, size_t pixelsWide, size_t pixelsHigh, size_t bitsPerPixel)
{
    if (!bytes)
        return NULL;

    Texture *texture = makeEmptyTexture();
    if (!texture)
        return NULL;

    ImageTexture *image = malloc(sizeof(ImageTexture));
    if (!image)
    {
        free(texture);
        return NULL;
    }

    image->bytes = bytes;
    image->pixelsWide = pixelsWide;
    image->pixelsHigh = pixelsHigh;
    image->bytesPerPixel = (bitsPerPixel / 8);
    image->bytesPerRow = image->bytesPerPixel * image->pixelsWide;

    texture->imageTexture = image;
    texture->value = imageTextureValue;
    texture->destructor = destructImageTexture;

    return texture;
}


// *** Reference Counting ***

static inline void incrementRefCounter(Texture *texture)
{
    if (texture)
        texture->refCount++;
}


static inline void decrementRefCounter(Texture *texture)
{
    if (!texture)
        return;

    if (--texture->refCount <= 0)
        texture->destructor(texture);
}


// *** Destructors ***

static inline void destructSolidColor(Texture *texture)
{
    if (texture)
    {
        if (texture->solidColor)
            free(texture->solidColor);
        free(texture);
    }
}


static inline void destructChecker(Texture *texture)
{
    if (texture)
    {
        // Don't free any even/odd textures as they may be in use with other
        // textures. Assume that they are freed elsewhere.
        if (texture->checker)
        {
            Texture *even = texture->checker->even;
            Texture *odd = texture->checker->odd;

            if (even)
                even->decrementRefCounter(even);
            if (odd)
                odd->decrementRefCounter(odd);

            free(texture->checker);
        }

        free(texture);
    }
}


static inline void destructImageTexture(Texture *texture)
{
    if (texture)
    {
        // Don't free the image bytes as we assume that they are freed elsewhere.
        if (texture->imageTexture)
            free(texture->imageTexture);

        free(texture);
    }
}


// *** Texture Functions ***

static inline Color3 solidColorValue(Texture *texture, double u, double v, Point3 *hitPt)
{
    return texture->solidColor->color;
}


static inline Color3 checkerTextureValue(Texture *texture, double u, double v, Point3 *hitPt)
{
    Checker *checker = texture->checker;

    const double sines = sin(10.0 * hitPt->x) * sin(10.0 * hitPt->y) * sin(10.0 * hitPt->z);

    if (sines < 0.0)
        return checker->odd->value(checker->odd, u, v, hitPt);
    else
        return checker->even->value(checker->even, u, v, hitPt);
}


static inline Color3 imageTextureValue(Texture *texture, double u, double v, Point3 *hitPt)
{
    ImageTexture *imageTexture = texture->imageTexture;
    uint8_t *bytes = imageTexture->bytes;

    if (!imageTexture->bytes)
        return color3(1, 1, 0);

    u = clamp(u, 0, 1);
    v = 1.0 - clamp(v, 0, 1); // Image is flipped!

    size_t i = (size_t)(u * imageTexture->pixelsWide);
    size_t j = (size_t)(v * imageTexture->pixelsHigh);

    if (i >= imageTexture->pixelsWide)
        i = imageTexture->pixelsWide - 1;
    if (j >= imageTexture->pixelsHigh)
        j = imageTexture->pixelsHigh - 1;

    const double invMaxByte = 1.0 / 255.0;

    uint8_t *pixels = bytes + (j * imageTexture->bytesPerRow + i * imageTexture->bytesPerPixel);

    return color3(pixels[0] * invMaxByte, pixels[1] * invMaxByte, pixels[2] * invMaxByte);
}
