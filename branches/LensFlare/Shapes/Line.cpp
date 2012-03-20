#include "Line.h"
#include <math.h>

Line::Line(const Point2D& p, int len, int angle) {
   p1 = p;
   if(angle == 90) {
      p2.set(p.x, p.y - len);
   }
   else if(angle == 270) {
      p2.set(p.x, p.y + len);
   }
   else {
      float rad = angle * M_PI / 180.0;
      p2.x = p1.x + (len * cos(rad));
      p2.y = p1.y + (len * sin(rad));
   }
}

bool Line::hit(float x, float y, Color& color) const {
   float f = (y - p1.y) * (p2.x - p1.x) - (x - p1.x) * (p2.y - p1.y);
   color.set(1, 1, 1, 1);
   return fabs(f) < 1.0E1;
}
