#ifndef _SPECULAR_TRANSMISSION
#define _SPECULAR_TRANSMISSION

#include "BRDF.h"
#include "Utility/Color.h"
#include "Fresnel.h"

class SpecularTransmission : public BRDF {

public:
   SpecularTransmission(float ei, float et);

   virtual Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
   void setColor(const Color& c) { color = c; }

private:
   Color color;
   float etai, etat;
   FresnelDielectric fresnel;
};

#endif
