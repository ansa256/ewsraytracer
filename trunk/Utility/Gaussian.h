#ifndef raytracer_Gaussian_h
#define raytracer_Gaussian_h

#include <SDL/SDL.h>

double* CreateGaussianFilter(double piSigma, double piAlpha, int& lSize);
SDL_Surface* Convolute(SDL_Surface* source, double* filter, int fSize);

#endif
