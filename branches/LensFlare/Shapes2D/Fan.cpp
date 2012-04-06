#include "Fan.h"
#include "Line.h"

Fan::Fan(int _x, int _y, int _length, float _angle, int _spread, int _height, const Color& c1, const Color& c2) :
   x(_x), y(_y), length(_length), angle(_angle), spread(_spread), height(_height), color1(c1), color2(c2)
{
}

void Fan::draw(SDL_Surface* surf) {
   float start = angle - spread / 2;
   float end = angle + spread / 2;

   for(float a = start; a <= end; a += 0.05) {
      Line line(x, y, length, a, height, color1, color2);
      line.draw(surf);
   }
}

