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
void setAlpha(SDL_Surface* surf, Uint8 alpha);

#endif

