//
//  Material.h
//  CPhoton
//
//  Created by Edward on 21/01/2023.
//

#ifndef Material_h
#define Material_h

#include "Ray.h"
#include "Vector3.h"
#include "HitRec.h"
#include "Texture.h"

#define makeSilverColor() 	color3(0.972, 0.960, 0.816)
#define makeGoldColor()		color3(1.000, 0.766, 0.336)
#define makeGreyColor()		color3(0.20, 0.26, 0.35)

typedef struct material_t
{
	Texture *albedo;
	Color3	emitted;	// For emitters only.
	double	fuzziness;	// Fuzziness of reflections.
	double 	indexOfRefraction;
	bool 	(*scatter)(Ray *, HitRec *hit, Ray *, Color3 *);
	
	int		refCount;	// Reference counter;
	void 	(*incrementRefCounter)(struct material_t *);
	void 	(*decrementRefCounter)(struct material_t *);
	void 	(*destructor)(struct material_t *);
}
Material;

Material *makeMetal(Texture *albedo, double fuzziness);
Material *makeLambertian(Texture *albedo);
Material *makeLambertianWithColor(Color3 color);
Material *makeDielectric(double indexOfRefraction);
Material *makeEmitter(Color3 color);

#endif /* Material_h */
