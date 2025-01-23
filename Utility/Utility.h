//
//  Utility.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#ifndef Utility_h
#define Utility_h

#include <math.h>
#include <stdlib.h>

#define min(a, b)			( (a) < (b) ? (a) : (b) )
#define max(a, b)			( (a) > (b) ? (a) : (b) )
#define clamp(x, low, high)	min( high, max(low, x) )

#define kDegreesToRadians	0.01745329252
#define kRadiansToDegrees	57.29577951

#define sind(x)		sin( (x) * kDegreesToRadians )
#define cosd(x)		cos( (x) * kDegreesToRadians )
#define tand(x)		tan( (x) * kDegreesToRadians )

#define asind(x)	( asin( (x) ) * kRadiansToDegrees )
#define acosd(x)	( acos( (x) ) * kRadiansToDegrees )
#define atand(x)	( atan( (x) ) * kRadiansToDegrees )

double randomDouble(void);
double randomDoubleRange(double min, double max);

#endif /* Utility_h */
