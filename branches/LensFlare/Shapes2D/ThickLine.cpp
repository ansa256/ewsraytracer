#include "ThickLine.h"
#include <math.h>

ThickLine::ThickLine(Uint16 _x1, Uint16 _y1, Uint16 _x2, Uint16 _y2, Uint16 w, const Color& c1, const Color& c2) :
   Line(_x1, _y1, _x2, _y2, c1, c2)
{
   hw = w / 2;
}

void ThickLine::draw(SDL_Surface* surf) {
   if(y1 == y2) {
      drawHorizontal(surf);
   }
   else if(x1 == x2) {
      drawVertical(surf);
   }
}

void ThickLine::drawHorizontal(SDL_Surface* surf) {
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

void ThickLine::drawVertical(SDL_Surface* surf) {
   int x = x1 - hw;
   int sx = x1;

   float denom = fabs(x1 - (x1 - hw));
   for(int i = 0; i < hw * 2 + 1; i++) {
      float alpha = 1.f - (fabs(x - sx) / denom);
      x1 = x2 = x;
      color1.alpha = alpha;
      color2.alpha = alpha;
      Line::draw(surf);
      x++;
   }
}
