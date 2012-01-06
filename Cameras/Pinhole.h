#ifndef _PINhOLE_H_
#define _PINhOLE_H_

#include "Camera.h"

class Pinhole : public Camera {
   
public:
   Pinhole(int w, int h);
   virtual ~Pinhole() {}

   virtual void renderScene(const SamplerBounds& bounds);
};

#endif
