#ifndef raytracer_Film_h
#define raytracer_Film_h

#include "Utility/Color.h"

class SDL_Surface;
class Filter;

struct Pixel {
   Pixel();
   Color getColor();
   void reset();

   float r, g, b, a;
   float weightSum;
};

class Film {
   
public:
   Film(int w, int h);
   ~Film();

   void addSample(float sx, float sy, const Color& c);
   void generateImage(SDL_Surface* surf);
   void reset();
   
private:
   int width, height;
   Filter* filter;
   Pixel* pixels;
};

#endif
