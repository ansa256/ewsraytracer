#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "Light.h"
#include "Math/Vector3D.h"

class Material;
class SamplerNew;

class Environment : public Light {

public:
   Environment();
   ~Environment();

   virtual void setHash(Hash* hash);
   virtual Vector3D getLightDirection(ShadeRecord& sr);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);

   virtual Color L(const ShadeRecord& sr);
   float pdf(const ShadeRecord& sr);

private:
   Material* material;
   float* samples;
   int numSamples, idx;
};

#endif
