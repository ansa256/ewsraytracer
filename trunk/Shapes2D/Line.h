#ifndef LensFlare_Line_h
#define LensFlare_Line_h

#include "Shape2D.h"
#include "Utility/Color.h"

class Line : public Shape2D {

public:
   Line(int _x1, int _y1, int _x2, int _y2, const Color& c1, const Color& c2);
   Line(int x, int y, int length, float angle, const Color& c1, const Color& c2);

   virtual void draw(SDL_Surface* surf);

protected:
   int x1, y1, x2, y2;
   Color color1, color2;
   bool swapAlpha;

   void horizontal(SDL_Surface* surf, int y);
   void vertical(SDL_Surface * surf, int x);
};

#endif

