#include "ThickLine.h"
#include <math.h>
#include "Line.h"
#include "Utility/Matrix2D.h"
#include "Murphy.h"

ThickLine::ThickLine(int _x1, int _y1, int _x2, int _y2, int w, const Color& c1, const Color& c2) :
      x1(_x1), y1(_y1), x2(_x2), y2(_y2), color1(c1), color2(c2)
{
   hw = w / 2;
}

ThickLine::ThickLine(int x, int y, int length, float angle, int w, const Color& c1, const Color& c2) {
   x1 = x;
   y1 = y;
   hw = w / 2;
   color1 = c1;
   color2 = c2;
   
   if(angle > 360) {
      angle -= 360;
   }
   if(angle < 0) {
      angle += 360;
   }

   angle *= M_PI / 180.f;
   x2 = (int)(length * cos(angle)) + x1;
   y2 = (int)(-length * sin(angle)) + y1;
}

void ThickLine::draw(SDL_Surface* surf) {
   if(y1 == y2) {
      drawHorizontal(surf);
   }
   else if(x1 == x2) {
      drawVertical(surf);
   }
   else {
//      drawAngle(surf);
      Murphy m(surf);
      m.wideline(x1, y1, x2, y2, hw+2);
   }
}

void ThickLine::drawHorizontal(SDL_Surface* surf) {
   int y = y1 - hw;
   int a1 = color1.alpha;
   int a2 = color2.alpha;

   float denom = fabs(y1 - (y1 - hw));
   for(int i = 0; i < hw * 2 + 1; i++) {
      float alpha = 1.f - (fabs(y - y1) / denom);
      color1.alpha = a1 * alpha;
      color2.alpha = a2 * alpha;
      Line l(x1, y, x2, y, color1, color2);
      l.setFilterNoDelete(filter);
      l.draw(surf);
      l.setFilterNoDelete(NULL);
      y++;
   }
}

void ThickLine::drawVertical(SDL_Surface* surf) {
   int x = x1 - hw;
   int a1 = color1.alpha;
   int a2 = color2.alpha;

   float denom = fabs(x1 - (x1 - hw));
   for(int i = 0; i < hw * 2 + 1; i++) {
      float alpha = 1.f - (fabs(x - x1) / denom);
      color1.alpha = a1 * alpha;
      color2.alpha = a2 * alpha;
      Line l(x, y1, x, y2, color1, color2);
      l.setFilterNoDelete(filter);
      l.draw(surf);
      l.setFilterNoDelete(NULL);
      x++;
   }
}
