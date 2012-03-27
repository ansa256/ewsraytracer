#ifndef LensFlare_Shape2D_h
#define LensFlare_Shape2D_h

#include <SDL/SDL.h>

class Shape2D {

public:
   virtual void draw(SDL_Surface* surf) = 0;
};

#endif
