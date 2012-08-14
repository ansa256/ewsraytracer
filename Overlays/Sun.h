#ifndef raytracer_Sun_h
#define raytracer_Sun_h

#include "Overlay.h"
#include "Utility/Color.h"

class Sun : public Overlay {

public:
   Sun() {}
   virtual ~Sun();

   virtual void setHash(Hash* hash);
   virtual void render(SDL_Surface* dest, int cx, int cy, int width, int height);

private:
   void create();

   int radius;
   int spikeLength;
   int numSpikes;
   float startAngle;
   RGBAColor color;
   SDL_Surface* surface;
   float sigma, alpha;
};

#endif
