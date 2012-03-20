#ifndef LensFlare_Shape2D_h
#define LensFlare_Shape2D_h

#include "Utility/Color.h"

class Shape2D {
   
public:
   Shape2D() {}
   virtual ~Shape2D() {}
   
   virtual bool hit(float x, float y, Color& color) const = 0;
};

#endif
