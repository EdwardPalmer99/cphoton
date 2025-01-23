//
//  main.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include <stdio.h>
#include "RayTracer.h"
#include "PPMWriter.h"
#include "Ray.h"
#include "Camera.h"
#include "Vector3.h"
#include "MengerCube.h"
#include "Scene.h"
#include "PolyhedralModels.h"
#include "Texture.h"
#include "Fractals3D.h"
#include <time.h>

static void runMengerCubeDefaultRender(void);

int main(int argc, const char * argv[])
{
	// Create the camera:
	const double aspectRatio	= 1.6;
	const int imageWidth	 	= 2560;
	const int imageHeight		= (int)(imageWidth / aspectRatio);
					
	Camera camera = makeCamera(45.0, aspectRatio, 1, 0, point3(-2.5, 2, 10), point3(0, 2, 0));

	Primitive *room = makeDarkKnightRoom(20, 16.0, 5);
	
	Scene *scene = makeScene();

	scene->addObject(scene, room);
	
	// Monolith:
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Primitive *cube = makeCube(point3(0.5 * i, 0.25 + 0.5 * j, 0), zeroVector(), 0.5, makeLambertian(makeSolidTexture(color3(.01, .01, .01))));
			
			scene->addObject(scene, cube);
		}
	}

	scene->markAsFinished(scene);
		
	PPMImage *outputImage = renderScene(scene, &camera, imageWidth, imageHeight, 500, 50);
		
	writeBinary16BitPPMImage(outputImage, "/Users/edward/Desktop/BatCave.ppm");
		
	scene->destructor(scene);
	freePPMImage(outputImage);
	
	return 0;
}



static void runMengerCubeDefaultRender(void)
{
	// Create the camera:
	const double aspectRatio	= 4.0 / 3.0;
	const int imageWidth	 	= 800;
	const int imageHeight		= (int)(imageWidth / aspectRatio);

	Camera camera = makeCamera(45.0, aspectRatio, 4, 0.0, point3(2, 5, 5), point3(0.2, 0.6, 1.0));

	// Create the textures:
	Texture *greyColor	 = makeSolidTexture(color3(0.20, 0.26, 0.35));
	Texture *goldColor	 = makeSolidTexture(makeGoldColor());

	// Create the materials:
	Material *greyMetal 		= makeMetal(greyColor, 0.2);
	Material *goldLambertian	= makeLambertian(goldColor);
	
	// Create the scene:
	Scene *scene = makeScene();

	Primitive *mengerSponge0 = makeMengerSponge(0, point3(-1.5, 0.5, -1.5), 1.0, goldLambertian);
	Primitive *mengerSponge1 = makeMengerSponge(1, point3(1.5, 0.5, -1.5), 1.0, goldLambertian);
	Primitive *mengerSponge3 = makeMengerSponge(3, point3(-1.5, 0.5, 1.5), 1.0, goldLambertian);
	Primitive *mengerSponge2 = makeMengerSponge(2, point3(1.5 , 0.5, 1.5), 1.0, goldLambertian);
	Primitive *mengerSponge4 = makeMengerSponge(4, point3(0, 0.5, 0), 1.0, goldLambertian);
	Primitive *plane 		 = makePlane(point3(0, 0, 0), vector3(0, 1, 0), greyMetal);

	scene->addObject(scene, mengerSponge0);
	scene->addObject(scene, mengerSponge1);
	scene->addObject(scene, mengerSponge2);
	scene->addObject(scene, mengerSponge3);
	scene->addObject(scene, mengerSponge4);
	scene->addObject(scene, plane);
	scene->markAsFinished(scene);

	// Render:
	PPMImage *outputImage = renderScene(scene, &camera, imageWidth, imageHeight, 100, 50);
	writeBinary16BitPPMImage(outputImage, "/Users/edward/Desktop/OutputImage.ppm");

	// Cleanup:
	freePPMImage(outputImage);
	scene->destructor(scene);
}
