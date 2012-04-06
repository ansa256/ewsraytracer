#ifndef LensFlare_Shape2D_h
#define LensFlare_Shape2D_h

#include <SDL/SDL.h>

class FalloffFilter;

class Shape2D {

public:
   Shape2D();
   virtual ~Shape2D();
   virtual void draw(SDL_Surface* surf) = 0;

protected:
   FalloffFilter* filter;
};

#endif
