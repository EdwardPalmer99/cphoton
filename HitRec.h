//
//  HitRec.h
//  CPhoton
//
//  Created by Edward on 21/01/2023.
//

#ifndef HitRec_h
#define HitRec_h

#include <stdlib.h>
#include <stdbool.h>
#include "Vector3.h"

struct material_t;

typedef struct
{
	double				t;			// Time of hit.
	Point3				hitPt;		// Location of hit.
	Vector3				normal;
	struct material_t	*material;
	bool 				frontFace;
	double 				u, v;		// Texture coordinates [0, 1].
}
HitRec;

#endif /* HitRec_h */
