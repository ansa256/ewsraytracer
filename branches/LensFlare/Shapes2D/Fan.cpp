#include "Fan.h"
#include "Line.h"
#include <math.h>

Fan::Fan(int _x, int _y, int _length, float _angle, int _spread, const Color& c1, const Color& c2) :
   x(_x), y(_y), length(_length), angle(_angle), spread(_spread), color1(c1), color2(c2)
{
}

void Fan::draw(SDL_Surface* surf) {
   float start = angle - spread / 2;
   float end = angle + spread / 2;
   float denom = fabs(start - angle);
   
   float a1 = color1.alpha;
   float a2 = color2.alpha;

   for(float a = start; a <= end; a += 0.05) {
      float alpha = 1.0 - fabs(a - angle) / denom;
      color1.alpha = a1 * alpha;
      color2.alpha = a2 * alpha;
      
      Line line(x, y, length, a, color1, color2);
      line.setFilterNoDelete(filter);
      line.draw(surf);
      line.setFilterNoDelete(NULL);
   }
}
