#ifndef LensFlare_ThickLine_h
#define LensFlare_ThickLine_h

#include "Line.h"

class ThickLine : public Line {

public:
   ThickLine(int _x1, int _y1, int _x2, int _y2, int w, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

private:
   int hw;

   void drawHorizontal(SDL_Surface* surf);
   void drawVertical(SDL_Surface* surf);
};

#endif

