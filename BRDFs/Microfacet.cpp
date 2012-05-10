#include "Microfacet.h"
#include "Fresnel.h"
#include <algorithm>
#include <math.h>
#include "Textures/Texture.h"

float Blinn::D(const Vector3D& normal, const Vector3D& wh, float m) const {
   float costhetah = normal.dot(wh);
//   return (exponent+2) * M_1_PI * 0.5 * powf(costhetah, exponent);
   float D1 = 1.f / (m * m * costhetah*costhetah*costhetah*costhetah);

   float a = acos(costhetah);
   float power = tan(a) / m;
   power = -(power * power);
   float D2 = exp(power);
   float D = D1 * D2;
   return D;
//   power = -(a * a);
//   return 5.f * exp(power);
}

Microfacet::Microfacet(Fresnel* f, MicrofacetDistribution* d) : 
   BRDF(BxDFType(REFLECT)), color(1, 1, 1), fresnel(f), distribution(d)
{
}

Microfacet::~Microfacet() {
   if(texture != NULL) {
      delete texture;
   }
}

Color Microfacet::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   float ndoto = fabs(sr.normal.dot(wo));
   float ndoti = fabs(sr.normal.dot(wi));

   if (ndoti == 0.f || ndoto == 0.f) {
      return BLACK;
   }

   Vector3D wh = wi + wo;
   if (wh.x == 0. && wh.y == 0. && wh.z == 0.) {
      return BLACK;
   }

   wh.normalize();
   float idoth = wi.dot(wh);
   float odoth = fabs(wo.dot(wh));
   float ndoth = fabs(sr.normal.dot(wh));
   float F = fresnel->evaluate(idoth);
   float G = min(1.f, min((2.f * ndoth * ndoto / odoth), (2.f * ndoth * ndoti / odoth)));
   
//   if(texture != NULL) {
//      R = texture->getColor(sr) * color;
//   }
   float D = (0.4 * distribution->D(sr.normal, wh, 0.4)) + (0.6 * distribution->D(sr.normal, wh, 0.2));
   float Rs = D * G * F / (4.f * ndoti * ndoto);
   return color * Rs;
}

Color Microfacet::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   return color;
}
