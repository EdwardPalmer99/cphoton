//
//  Texture.h
//  CPhoton
//
//  Created by Edward on 29/01/2023.
//

#ifndef Texture_h
#define Texture_h

#include <math.h>
#include "Vector3.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct texture_t 		Texture;
typedef struct solidColor_t		SolidColor;
typedef struct checker_t 		Checker;
typedef struct imageTexture_t 	ImageTexture;

struct texture_t
{
	union
	{
		SolidColor		*solidColor;
		Checker			*checker;
		ImageTexture	*imageTexture;
	};

	Color3 (*value)(Texture *, double u, double v, Point3 *);
	void (*destructor)(Texture *);
	void (*incrementRefCounter)(Texture *);
	void (*decrementRefCounter)(Texture *);
	
	int refCount;
};

Texture *makeSolidTexture(Color3 color);
Texture *makeCheckerTexture(Texture *even, Texture *odd);
Texture *makeImageTexture(uint8_t *bytes, size_t pixelsWide, size_t pixelsHigh, size_t bytesPerPixel);
Texture *makeCheckerTextureWithColors(Color3 color1, Color3 color2);

#endif /* Texture_h */
