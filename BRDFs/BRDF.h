#ifndef __BRDF__
#define __BRDF__

#include "Utility/Color.h"
#include "Math/Vector3D.h"
#include "Utility/ShadeRecord.h"

class BRDF {

public:
   BRDF() {}
   virtual ~BRDF() {}

   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const { return BLACK; }
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const { return BLACK; }
   virtual float getAlpha(const ShadeRecord& sr) const { return 1.0; }
};

#endif
