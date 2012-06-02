#ifndef _SPECULAR_TRANSMISSION
#define _SPECULAR_TRANSMISSION

#include "BRDF.h"
#include "Utility/Color.h"
#include "Fresnel.h"

class SpecularTransmission : public BRDF {

public:
   SpecularTransmission(const Color& c, float ei, float et);

   virtual Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

private:
   Color color;
   float etai, etat;
   FresnelDielectric fresnel;
};

#endif