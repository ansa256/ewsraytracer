#ifndef _DIRECT_INTEGRATOR_H_
#define _DIRECT_INTEGRATOR_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"

class Light;

class DirectIntegrator {

public:
   DirectIntegrator();

   Color shade(ShadeRecord& sr, const Ray& ray);

private:
   Color estimateDirect(ShadeRecord& sr, Light* light, const Vector3D& wo, float* samples, int s);
};

#endif
