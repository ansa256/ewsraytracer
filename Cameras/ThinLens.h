#ifndef _THIN_LENS_H_
#define _THIN_LENS_H_

#include "Camera.h"

class Hash;

class ThinLens : public Camera {
   
public:
   ThinLens(int w, int h);
   virtual ~ThinLens();
   
   virtual void setHash(Hash* hash);
   virtual void renderScene(const SamplerBounds& bounds);
   
private:
   float lensRadius;
   // Distance from eye to focal plane
   float f;
};

#endif
