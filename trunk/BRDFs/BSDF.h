#ifndef _BSDF_H_
#define _BSDF_H_

#include "BRDF.h"
#include <vector>

using namespace std;

class BSDF : public BRDF {

public:
   virtual ~BSDF();
   
   void addBRDF(BRDF* brdf);

   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const;
   virtual float getAlpha(const ShadeRecord& sr) const;

private:
   vector<BRDF*> brdfs;
};

#endif
