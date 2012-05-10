#include "PerfectSpecular.h"
#include "Fresnel.h"

PerfectSpecular::PerfectSpecular() : BRDF(BxDFType(REFLECT | SPECULAR)), kr(0.75), color(1.0, 1.0, 1.0) {
   fresnel = new FresnelDielectric(0, kr);
}

void PerfectSpecular::setKr(const float k) { 
   kr = k; 
   delete fresnel;
   fresnel = new FresnelConductor(309.95, 1.84);
}

//Color PerfectSpecular::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi) const {
//   float ndotwo = sr.normal.dot(wo);
//
//   // Set wi = the reflection of wo throught the surface normal
//   wi = -wo + sr.normal * 2.0 * ndotwo;
//   return color * kr / fabs(sr.normal.dot(wi));
//}

Color PerfectSpecular::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
   pdf = 1.f;
   float ndotwo = sr.normal.dot(wo);

   // Set wi = the reflection of wo throught the surface normal
   wi = -wo + sr.normal * 2.0 * ndotwo;
   
   float ndotwi = fabs(sr.normal.dot(wi));
   return color * fresnel->evaluate(ndotwo) / ndotwi;
}
