#include "GlossySpecular.h"
#include "Textures/Texture.h"
#include <math.h>

GlossySpecular::GlossySpecular() : exp(0), color(1, 1, 1), specularMask(NULL) {
}

GlossySpecular::~GlossySpecular() {
   if(specularMask != NULL) {
      delete specularMask;
   }
}

Color GlossySpecular::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   if(specularMask != NULL) {
      Color c = specularMask->getColor(sr);
      if(c == BLACK) {
         return BLACK;
      }
   }

   float ndotwi = sr.normal.dot(wi);
   Vector3D r(wi * -1.0 + sr.normal * 2.0 * ndotwi);
   float rdotwo = r.dot(wo);

   if (rdotwo > 0.0) {
      return color * pow(rdotwo, exp);
   }

   return BLACK;
}

Color GlossySpecular::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   return BLACK;
}
