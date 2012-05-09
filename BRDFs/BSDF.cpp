#include "BSDF.h"

typedef vector<BRDF*>::const_iterator BRDFIter;

BSDF::~BSDF() {
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      delete *it;
   }
   brdfs.clear();
}

void BSDF::addBRDF(BRDF* brdf) {
   brdfs.push_back(brdf);
}

Color BSDF::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   Color f;
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      f += (*it)->f(sr, wo, wi);
   }
   return f;
}

Color BSDF::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   Color rho;
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      rho += (*it)->rho(sr, wo);
   }
   return rho;
}

float BSDF::getAlpha(const ShadeRecord& sr) const {
   float a = 1.f;
   for(BRDFIter it = brdfs.begin(); it != brdfs.end(); ++it) {
      a *= (*it)->getAlpha(sr);
   }
   return a;
}
