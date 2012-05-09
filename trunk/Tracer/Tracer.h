#ifndef _TRACER_H_
#define _TRACER_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"
#include "Textures/Texture.h"
#include "DirectIntegrator.h"

class Tracer {

public:
   Tracer();
   ~Tracer();

   Color traceRay(const Ray& ray, const int depth);

   void setBackgroundColor(const Color& c) { bgColor = c; }
   void setBackgroundTexture(Texture* t) { texture = t; }
   void setMaxDepth(int d) { maxDepth = d; }

protected:
   Color bgColor;
   Texture* texture;
   int maxDepth;
   DirectIntegrator integrator;
};

#endif
