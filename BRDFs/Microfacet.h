#ifndef raytracer_Microfacet_h
#define raytracer_Microfacet_h

#include "BRDF.h"
#include "Math/Vector3D.h"

class Fresnel;
class Texture;

class MicrofacetDistribution {
  
public:
   virtual ~MicrofacetDistribution() { }
   virtual float D(const Vector3D& normal, const Vector3D& wh, float m) const = 0;
   
   bool SameHemisphere(const Vector3D& w, const Vector3D& wp) const {
      return w.z * wp.z > 0.f;
   }
};

class Blinn : public MicrofacetDistribution {
  
public:
   Blinn() {}

   float D(const Vector3D& normal, const Vector3D& wh, float m) const;

private:
   float exponent;
};

class Microfacet : public BRDF {
   
public:
   Microfacet(Fresnel* f, MicrofacetDistribution* d);
   virtual ~Microfacet();
   
   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const;
   
   void setColor(const Color& c) { color = c; }
   
private:
   Color color;
   Fresnel* fresnel;
   MicrofacetDistribution* distribution;
};

#endif
