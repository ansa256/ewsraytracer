#include "SpecularTransmission.h"

SpecularTransmission::SpecularTransmission(const Color& c, float ei, float et) :
   BRDF(BxDFType(TRANSMIT | SPECULAR)),
   color(c),
   etai(ei),
   etat(et),
   fresnel(ei, et)
{
}

Color SpecularTransmission::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
   float ndotwo = sr.normal.dot(wo);
   bool entering = ndotwo > 0.;
//   float ei = etai, et = etat;
   
//   if (!entering) {
//      swap(ei, et);
//   }
//   float eta = ei / et;

   // Compute transmitted ray direction
/*   
   float sini2 = max(0.f, 1.f - ndotwo*ndotwo);
   float eta = ei / et;
   float sint2 = eta * eta * sini2;

   // Handle total internal reflection for transmission
   if (sint2 >= 1.) {
      return ZERO;
   }

   float cost = sqrtf(max(0.f, 1.f - sint2));
   if (entering) cost = -cost;
   float sintOverSini = eta;
   wi = Vector3D(sintOverSini * -wo.x, sintOverSini * -wo.y, cost);
*/
   
   float eta = 1.6f;
   if(!entering) {
      eta = 1.f / eta;
   }
   float temp = 1.f - (1.f - ndotwo * ndotwo) / (eta * eta);
   float cosTheta2 = sqrtf(temp);
   wi = -wo / eta - sr.normal * (cosTheta2 - ndotwo / eta);
   
   pdf = 1.f;
//   float F = fresnel.evaluate(ndotwo);
//   return color * (1.f - F) / fabs(sr.normal.dot(wi));
   return color / fabs(sr.normal.dot(wi));
}
