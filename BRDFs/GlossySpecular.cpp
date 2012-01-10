#include "GlossySpecular.h"
#include <math.h>

GlossySpecular::GlossySpecular() : exp(0), color(1, 1, 1) {
}

GlossySpecular::~GlossySpecular() {
}

void GlossySpecular::setColor(const float r, const float g, const float b) {
   color.set(r, g, b);
}

Color GlossySpecular::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
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
