#include "Arc.h"
#include "Utility/Math.h"
#include <math.h>

Arc::Arc(int x, int y, int r, float a1, float a2) : cx(x), cy(y), radius(r), angle1(a1), angle2(a2), 
   innerColor(Color(1, 1, 1, 1)), outerColor(Color(0, 0, 1, 0)) 
{   
}

Arc::~Arc() {
   
}

bool Arc::hit(float x, float y, Color& color) const {
   float x2 = pow(x - cx, 2);
   float y2 = pow(y - cy, 2);
   
   float dist = sqrtf(x2 + y2);
   float p = 1.0 - (radius - dist) / radius;
   color = lerp(p, innerColor, outerColor);
   
   if(dist <= radius) {
      float angle = acos((x-cx) / dist) * 180.0 / M_PI;
      if((angle >= angle1) && (angle < angle2)) {
         return true;
      } 
   }
   return false;
}

