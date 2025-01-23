//
//  RayTracer.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "RayTracer.h"

#define kMinHitTime		0.001	// Positive tmin fixes shadow acne.

static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth);



PPMImage *renderScene(Scene *scene, Camera *camera, int imageWidth, int imageHeight,
					  int samplesPerPixel, int maxDepth)
{
	if (!scene || !scene->sceneNode || !camera) return NULL;
	
	Primitive *sceneNode = scene->sceneNode;	// Contains all objects.
	
	PPMImage *image = makePPMImage(imageWidth, imageHeight);
	if (!image) return NULL;
	
	const double invSamplesPerPivel = 1.0 / (double)samplesPerPixel;
	
	__block int numRowsRendered = 0;
	
	dispatch_apply(imageHeight, DISPATCH_APPLY_AUTO, ^(size_t iRow)
	{
		for (int iCol = 0; iCol < imageWidth; iCol++)
		{
			Color3 pixelColor = color3(0, 0, 0);
			
			// Sampling:
			for (int iSample = 0; iSample < samplesPerPixel; iSample++)
			{
				const double u = (iCol + randomDouble()) / (double)(imageWidth - 1);
				const double v = (iRow + randomDouble()) / (double)(imageHeight - 1);
				
				// Generate a new camera ray:
				Ray ray = getRay(camera, u, v);
				
				pixelColor = addVectors(pixelColor, rayColor(&ray, sceneNode, maxDepth));
			}
			
			// Set the image's pixel to the average value:
			image->pixelValue[iRow][iCol] = scaleVector(pixelColor, invSamplesPerPivel);
		}
		
		numRowsRendered++;
		printf("Progress: %3.2lf %%\n", (100.0 * numRowsRendered) / (double)imageHeight);
	});

	return image;
}



static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth)
{
	HitRec hit;
	
	if (depth <= 0) return color3(0, 0, 0);	// Exceeded ray bounce limit.
	
	if (objectsBVH->hit(objectsBVH, ray, kMinHitTime, INFINITY, &hit))
	{
		Ray scatteredRay;
		Color3 attenuation;
		Color3 emitted	= hit.material->emitted;
		
		if (hit.material->scatter(ray, &hit, &scatteredRay, &attenuation))
		{
			Color3 outputColor = rayColor(&scatteredRay, objectsBVH, depth - 1);
			
			return addVectors(emitted, multiplyColors(outputColor, attenuation));
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		// Didn't hit anything. Return the background color for the sky:
		const double t = 0.5 * (unitVector(ray->direction).y + 1.0);
		
		Color3 whiteComponent	= scaleVector(color3(1, 1, 1), 1 - t);
		Color3 blueComponent	= scaleVector(color3(0.5, 0.7, 1.0), t);
		
		return addVectors(whiteComponent, blueComponent);
	}
}
