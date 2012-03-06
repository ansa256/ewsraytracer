#ifndef _PHONG_H_
#define _PHONG_H_

#include "Material.h"

class Lambertian;
class GlossySpecular;

class Phong : public Material {

public:
   Phong();
   virtual ~Phong();

   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   virtual float getAlpha(const ShadeRecord& sr, const Ray& ray) const;

private:
   void setTexture(string texture);

   Lambertian* ambientBRDF;
   Lambertian* diffuseBRDF;
   GlossySpecular* specularBRDF;
   float kd;
};

#endif
