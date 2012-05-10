#include "PerfectSpecular.h"
#include "Fresnel.h"

PerfectSpecular::PerfectSpecular() : BRDF(), kr(0.75), color(1.0, 1.0, 1.0) {
   fresnel = new FresnelDielectric(0, kr);
}

void PerfectSpecular::setKr(const float k) { 
   kr = k; 
   delete fresnel;
   fresnel = new FresnelDielectric(0, kr);
}

//Color PerfectSpecular::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi) const {
//   float ndotwo = sr.normal.dot(wo);
//
//   // Set wi = the reflection of wo throught the surface normal
//   wi = -wo + sr.normal * 2.0 * ndotwo;
//   return color * kr / fabs(sr.normal.dot(wi));
//}

Color PerfectSpecular::sample_f(const ShadeRecord &sr, const Vector3D &wo, Vector3D &wi) const {
   float ndotwi = fabs(sr.normal.dot(wi));
   float ndotwo = sr.normal.dot(wo);
   return color * fresnel->evaluate(ndotwo) / ndotwi;
}