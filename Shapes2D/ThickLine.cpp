#include "ThickLine.h"
#include <math.h>

ThickLine::ThickLine(Uint16 _x1, Uint16 _y1, Uint16 _x2, Uint16 _y2, Uint16 w, const Color& c1, const Color& c2) :
   Line(_x1, _y1, _x2, _y2, c1, c2)
{
   hw = w / 2;
}

void ThickLine::draw(SDL_Surface* surf) {
   int y = y1 - hw;
   int sy = y1;
   int a1 = color1.alpha;
   int a2 = color2.alpha;

   float denom = fabs(y1 - (y1 - hw));
   for(int i = 0; i < hw * 2 + 1; i++) {
      float alpha = 1.f - (fabs(y - sy) / denom);
      y1 = y2 = y;
      color1.alpha = a1 * alpha;
      color2.alpha = a2 * alpha;
      Line::draw(surf);
      y++;
   }
}
