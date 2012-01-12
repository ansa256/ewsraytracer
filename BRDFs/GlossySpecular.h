#ifndef __GLOSSY_SPECULAR__
#define __GLOSSY_SPECULAR__

#include "BRDF.h"

class Texture;

class GlossySpecular : public BRDF {

public:
   GlossySpecular();
   virtual ~GlossySpecular();

   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const;

   void setExp(const float e) { exp = e; }
   void setColor(const Color& c) { color = c; }
   void setColor(const float r, const float g, const float b);
   void setSpecularMask(Texture* mask) { specularMask = mask; }

private:
   float exp;
   Color color;
   Texture* specularMask;
};

#endif
