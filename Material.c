//
//  Material.c
//  CPhoton
//
//  Created by Edward on 21/01/2023.
//

#include "Material.h"

static inline Material *makeEmptyMaterial(void);

static inline void incrementRefCounter(Material *);
static inline void decrementRefCounter(Material *);
static inline void destructMaterial(Material *material);

static inline bool scatterRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation);
static inline bool reflectRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation);
static inline bool refractRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation);
static inline bool absorbRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation);

static inline double reflectance(double cosine, double refractionRatio);
static inline Vector3 reflect(Vector3 v, Vector3 n);
static inline Vector3 refract(Vector3 vin, Vector3 n, double refractionRatio);


#pragma mark - *** Make Materials ***

static inline Material *makeEmptyMaterial(void)
{
	Material *emptyMaterial = malloc(sizeof(Material));
	if (!emptyMaterial) return NULL;
	
	emptyMaterial->refCount	= 0;
	emptyMaterial->incrementRefCounter = incrementRefCounter;
	emptyMaterial->decrementRefCounter = decrementRefCounter;
	
	emptyMaterial->emitted		= color3(0, 0, 0);
	emptyMaterial->fuzziness	= 0.0;
	emptyMaterial->albedo 		= NULL;
	emptyMaterial->scatter		= absorbRay;
	emptyMaterial->destructor	= destructMaterial;
	
	return emptyMaterial;
}



Material *makeEmitter(Color3 color)
{
	Material *emitter = makeEmptyMaterial();
	if (!emitter) return NULL;
	
	emitter->emitted = color;
	emitter->scatter = absorbRay;
	
	return emitter;
}



Material *makeMetal(Texture *albedo, double fuzziness)
{
	if (!albedo) return NULL;
	
	Material *metal = makeEmptyMaterial();
	if (!metal) return NULL;
		
	metal->albedo		= albedo;
	metal->fuzziness	= clamp(fuzziness, 0, 1);
	metal->scatter		= reflectRay;
		
	albedo->incrementRefCounter(albedo);

	return metal;
}



Material *makeLambertian(Texture *albedo)
{
	if (!albedo) return NULL;
	
	Material *lambertian = makeEmptyMaterial();
	if (!lambertian) return NULL;
	
	lambertian->albedo 		= albedo;
	lambertian->scatter		= scatterRay;
	
	albedo->incrementRefCounter(albedo);
	
	return lambertian;
}



Material *makeLambertianWithColor(Color3 color)
{
	return makeLambertian(makeSolidTexture(color));
}



Material *makeDielectric(double indexOfRefraction)
{
	Material *dielectric = makeEmptyMaterial();
	if (!dielectric) return NULL;
	
	dielectric->indexOfRefraction = indexOfRefraction;
	dielectric->scatter = refractRay;
	
	return dielectric;
}


#pragma mark - *** Destructors ***

static inline void destructMaterial(Material *material)
{
	if (!material) return;
	
	Texture *albedo = material->albedo;
	
	if (albedo) albedo->decrementRefCounter(albedo);

	free(material);
}



#pragma mark - *** Reference Counting ***

static inline void incrementRefCounter(Material *material)
{
	if (material) ++material->refCount;
}


static inline void decrementRefCounter(Material *material)
{
	if (!material) return;

	if (--material->refCount <= 0) material->destructor(material);
}



#pragma mark -

static inline bool absorbRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation)
{
	return false;
}


static inline bool scatterRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation)
{
	// Compute random scattering direction:
	Vector3 scatterDirection = addVectors(hit->normal, randomUnitVector());
	
	// If random vector is in opposite direction to normal they will sum to
	// a zero vector. Set to normal direction if this happens.
	if (isNearlyZero(scatterDirection))
	{
		scatterDirection = hit->normal;
	}
	
	Texture *albedo = hit->material->albedo;
	
	*scatteredRay = makeRay(hit->hitPt, scatterDirection);
	*attenuation  = albedo->value(albedo, hit->u, hit->v, &hit->hitPt);
	
	return true;
}


static inline bool reflectRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation)
{
	Vector3 reflectDirection = reflect(unitVector(incidentRay->direction), hit->normal);
	
	// Fuzzy reflections are created by choosing a new endpoint in a unit sphere.
	if (hit->material->fuzziness > 0.0)
	{
		Vector3 changeToVector = scaleVector(randomUnitSphereVector(), hit->material->fuzziness);
		reflectDirection = addVectors(reflectDirection, changeToVector);
	}
	
	Texture *albedo = hit->material->albedo;
	
	*scatteredRay = makeRay(hit->hitPt, reflectDirection);
	*attenuation  = albedo->value(albedo, hit->u, hit->v, &hit->hitPt);
	
	// Make sure that the scattered ray is not scattering into the object:
	return (dot(reflectDirection, hit->normal) > 0.0);
}


static inline bool refractRay(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation)
{
	Vector3 unitDirection = unitVector(incidentRay->direction);
	
	// Angle theta is the angle between the normal and incident ray.
	const double cosTheta = dot(flipVector(unitDirection), hit->normal);
	const double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
	// If it's the front face, we're going into the object otherwise we're leaving
	// and exiting into the air.
	const double ir = hit->material->indexOfRefraction;
	const double refractionRatio = hit->frontFace ? (1.0 / ir) : ir;
	
	// Total internal reflection:
	bool cannotRefract = (refractionRatio * sinTheta > 1.0);
	
	Vector3 direction;
	
	if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
		direction = reflect(unitDirection, hit->normal);
	else
		direction = refract(unitDirection, hit->normal, refractionRatio);
	
	*scatteredRay	= makeRay(hit->hitPt, direction);
	*attenuation	= color3(1.0, 1.0, 1.0);
	
	return true;
}


/// Schlick's approximation for reflectance. Glass has reflectivity that varies
/// with angle. This is similar to looking at a window at a steep angle---it
/// mostly reflects. This will be when the cosine is small.
static inline double reflectance(double cosine, double refractionRatio)
{
	double r0 = (1.0 - refractionRatio) / (1.0 + refractionRatio);
	
	r0 = r0 * r0;
	
	return r0 + (1.0 - r0) * pow(1.0 - cosine, 5);
}


static inline Vector3 reflect(Vector3 v, Vector3 n)
{
	// Reflected vector: v - 2*(v.n)*n
	return subtractVectors(v, scaleVector(n, 2.0 * dot(v, n)));
}


static inline Vector3 refract(Vector3 vin, Vector3 n, double refractionRatio)
{
	Vector3 vinParallel			= scaleVector(n, dot(vin, n));	// Parallel to normal component, n.
	Vector3 vinPerpendicular	= subtractVectors(vin, vinParallel);
	
	Vector3 voutPerpendicular	= scaleVector(vinPerpendicular, refractionRatio);
	Vector3 voutParallel		= scaleVector(n, -sqrt(1.0 - lengthSquared(voutPerpendicular)));
	
	return addVectors(voutPerpendicular, voutParallel);
}

