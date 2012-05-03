#ifndef raytracer_Metal_h
#define raytracer_Metal_h

#include "Material.h"

class Microfacet;
class Lambertian;

class Metal : public Material {

public:
   Metal();
   virtual ~Metal();
   
   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   virtual float getAlpha(const ShadeRecord& sr, const Ray& ray) const;

private:
   void setTexture(string texture);

   Microfacet* microfacet;
   Lambertian* ambient;
   Lambertian* diffuse;
   double kd;
};

#endif
