#ifndef LensFlare_ThickLine_h
#define LensFlare_ThickLine_h

#include "Line.h"

class ThickLine : public Line {

public:
   ThickLine(Uint16 _x1, Uint16 _y1, Uint16 _x2, Uint16 _y2, Uint16 w, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

private:
   Uint16 hw;
};

#endif
