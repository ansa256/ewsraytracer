#ifndef LensFlare_SDL_Utility_h
#define LensFlare_SDL_Utility_h

#include <SDL/SDL.h>
#include "Color.h"

SDL_Surface* createSurface(int w, int h);
void setColor(SDL_Surface* surf, int x, int y, const Color& c);
void setColor(SDL_Surface* surf, int x, int y, Uint32 color);
void setBlendColor(SDL_Surface *dst, Sint16 x, Sint16 y, const Color& c);
void setBlendColor(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha);

template<class T>
inline T lerp(const float f, const T a, const T b) {
   return a + (b - a) * f;
}

#endif
