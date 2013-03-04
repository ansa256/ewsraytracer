#ifndef LensFlare_convoluteCL_h
#define LensFlare_convoluteCL_h

#include <SDL/SDL.h>

SDL_Surface* convoluteCL(SDL_Surface* source, float* filter, int fsize);

#endif
