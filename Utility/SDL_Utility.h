#ifndef LensFlare_SDL_Utility_h
#define LensFlare_SDL_Utility_h

#include <SDL/SDL.h>
#include <png.h>
#include "Color.h"

SDL_Surface* createSurface(int w, int h);
void setColor(SDL_Surface* surf, int x, int y, const Color& c);
void setColor(SDL_Surface* surf, int x, int y, Uint32 color);
void setBlendColor(SDL_Surface *surf, int x, int y, const Color& c);
void setBlendColor(SDL_Surface* surf, Uint32* pixel, const Color& c);
void setBlendColor(SDL_Surface * surf, int x, int y, const Color& c, int weight);
void setAlpha(SDL_Surface* surf, Uint8 alpha);

void user_error_fn(png_structp ctx, png_const_charp str);
void user_warning_fn(png_structp ctx, png_const_charp str);
int png_colortype_from_surface(SDL_Surface *surface);
void saveImage(SDL_Surface* surface, const char* fname);

#endif

