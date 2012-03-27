#ifndef LensFlare_Line_h
#define LensFlare_Line_h

#include "Shape2D.h"
#include "Utility/Color.h"

class Line : public Shape2D {

public:
   Line(Uint16 _x1, Uint16 _y1, Uint16 _x2, Uint16 _y2, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

protected:
   Uint16 x1, y1, x2, y2;
   Color color1, color2;

   void horizontal(SDL_Surface* surf, Uint16 y);
   void vertical(SDL_Surface * surf, Uint16 x);
};

#endif
