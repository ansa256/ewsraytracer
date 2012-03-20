#include "Circle.h"
#include "Utility/Math.h"
#include <math.h>

Circle::Circle(int x, int y, int r) : cx(x), cy(y), radius(r), innerColor(Color(1, 1, 1, 1)), outerColor(Color(0, 0, 1, 0)) {
   
}

Circle::~Circle() {
   
}

bool Circle::hit(float x, float y, Color& color) const {
   float x2 = pow(x - cx, 2);
   float y2 = pow(y - cy, 2);
   
   float dist = sqrtf(x2 + y2);
   float p = 1.0 - (radius - dist) / radius;
   color = lerp(p, innerColor, outerColor);
   
   return dist <= radius;
}
