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

   virtual void setColor(float r, float g, float b);
   virtual void setTexture(string texture);

   void setAmbientColor(const Color& c);
   void setDiffuseColor(const Color& c);
   void setSpecularColor(const Color& c);
   void setSpecularHighlight(float sh);

private:
   Lambertian* ambientBRDF;
   Lambertian* diffuseBRDF;
   GlossySpecular* specularBRDF;
   float kd;
};

#endif
