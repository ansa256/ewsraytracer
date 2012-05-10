#ifndef __BRDF__
#define __BRDF__

#include "Utility/Color.h"
#include "Math/Vector3D.h"
#include "Utility/ShadeRecord.h"

enum BxDFType {
   REFLECT =   1 << 0,
   TRANSMIT =  1 << 1,
   DIFFUSE =   1 << 2,
   SPECULAR =  1 << 3,
   GLOSSY =    1 << 4
};

class BRDF {

public:
   BRDF(BxDFType t) : type(t) {}
   virtual ~BRDF() {}

   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const { return BLACK; }
   virtual Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const { return BLACK; }
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const { return BLACK; }
   virtual float getAlpha(const ShadeRecord& sr) const { return 1.0; }
   
   bool matches(BxDFType flags) const { return (type & flags) == type; }

   const BxDFType type;
};

#endif
