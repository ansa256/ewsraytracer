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
   Blinn(float e = 4.0) : exponent(e) {}
   float D(const Vector3D& normal, const Vector3D& wh, float m) const;

private:
   float exponent;
};

class Microfacet : public BRDF {
   
public:
   Microfacet(Fresnel* f, MicrofacetDistribution* d);
   virtual ~Microfacet() {}
   
   void setColor(const Color& c) { color = c; }
   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const = 0;
   
protected:
   /**
    * Geometric attenuation term. The fraction of microfacets that are masked or shadowed.
    */
   float G(const ShadeRecord& sr, const Vector3D& wh, const Vector3D& wo, const Vector3D& wi) const;

   Color color;
   Fresnel* fresnel;
   MicrofacetDistribution* distribution;
};

class CookTarrance : public Microfacet {
   
public:
   CookTarrance(Fresnel* f, MicrofacetDistribution* d) : Microfacet(f, d) {}
   virtual ~CookTarrance() {}
   
   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;   
};

#endif
