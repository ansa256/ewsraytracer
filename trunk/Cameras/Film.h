#ifndef raytracer_Film_h
#define raytracer_Film_h

#include "Utility/Color.h"
#include "Math/Point2D.h"

class SDL_Surface;
class Filter;

struct Pixel {
   Pixel();
   Color getColor();

   float r, g, b;
   float weightSum;
};

class Film {
   
public:
   Film(int w, int h);
   ~Film();

   void addSample(double sx, double sy, const Color& c);
   void generateImage(SDL_Surface* surf);
   
private:
   int width, height;
   Filter* filter;
   Pixel* pixels;
};

#endif
