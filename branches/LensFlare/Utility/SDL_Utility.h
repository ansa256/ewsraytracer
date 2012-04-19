#ifndef LensFlare_SDL_Utility_h
#define LensFlare_SDL_Utility_h

#include <SDL/SDL.h>
#include "Color.h"

SDL_Surface* createSurface(int w, int h);
void setColor(SDL_Surface* surf, int x, int y, const Color& c);
void setColor(SDL_Surface* surf, int x, int y, Uint32 color);
void setBlendColor(SDL_Surface *surf, int x, int y, const Color& c);
void setBlendColor(SDL_Surface* surf, Uint32* pixel, const Color& c);
void setBlendColor(SDL_Surface * surf, int x, int y, const Color& c, int weight);

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void setPixel(SDL_Surface* s, int x, int y, Uint32 p);

#endif

