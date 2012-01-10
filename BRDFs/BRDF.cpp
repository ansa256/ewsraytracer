#include "BRDF.h"
#include "Samplers/Sampler.h"
#include "Math/Maths.h"

BRDF::BRDF() : sampler(NULL) {
}

BRDF::~BRDF() {
   if(sampler) {
      delete sampler;
   }
}

void BRDF::setSampler(Sampler* s) {
   sampler = s;
   // map to hemispher
}

Color BRDF::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   return BLACK;
}
	
Color BRDF::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   return BLACK;
}

float BRDF::fresnelDielectic(float cosi, float etai, float etat) const {
   cosi = clamp(cosi, -1.f, 1.f);
   
   if(cosi <= 0.f) {
      swap(etai, etat);
   }
   
   float sint = etai / etat * sqrtf(max(0.f, 1.f - cosi * cosi));
   if(sint >= 1.f) {
      return 1.f;
   }
   
   float cost = sqrtf(max(0.f, 1.f - sint * sint));
   cosi = fabs(cosi);
   
   float parallel = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
   float perp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
   return (parallel + perp) * 0.5f;
}

float BRDF::fresnelConductor(float cosi, float eta, float k) const {
   cosi = fabs(cosi);
   float tmp = (eta * eta + k * k) * cosi * cosi;
   float parallel = (tmp - (2.f * eta * cosi) + 1) / (tmp + (2.f * eta * cosi) + 1);
   
   tmp = eta * eta * k * k;
   float perp = (tmp - (2.f * eta * cosi) + cosi * cosi) / (tmp + (2.f * eta * cosi) + cosi * cosi);
   return (parallel + perp) * 0.5f;
}
