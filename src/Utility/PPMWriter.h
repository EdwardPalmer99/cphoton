//
//  PPMWriter.h
//  CMandelbrot
//
//  Created by Edward on 02/11/2022.
//

#ifndef PPMWriter_h
#define PPMWriter_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Vector3.h"

#define NUM_PIXELS(image)	(image)->width * (image)->height

typedef struct
{
	int		width;
	int 	height;
	Color3 	*pixels;
	Color3 	**pixelValue;
}
PPMImage;

PPMImage *makePPMImage(int width, int height);
void freePPMImage(PPMImage *image);

bool writePPMImage(PPMImage *image, const char *fpath);
bool writeBinaryPPMImage(PPMImage *image, const char *fpath);
bool writeBinary16BitPPMImage(PPMImage *image, const char *fpath);
bool writeTo32BitARGBBuffer(PPMImage *image, void *basePtr, size_t bytesPerRow);

void clearImage(PPMImage *image);
void fadeImage(PPMImage *image, double fadeFactor);
void copyImage(PPMImage *imageDst, PPMImage *imageSrc);

#endif /* PPMWriter_h */
