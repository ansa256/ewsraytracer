#include "SDL_Utility.h"

SDL_Surface* createSurface(int w, int h) {
   SDL_Surface *surface;
   Uint32 rmask, gmask, bmask, amask;

   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
    on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif

   surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
   if(surface == NULL) {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      exit(1);
   }

   return surface;
}

void setColor(SDL_Surface* surf, int x, int y, const Color& c) {
   setColor(surf, x, y, SDL_MapRGBA(surf->format, c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha()));
}

void setColor(SDL_Surface* surf, int x, int y, Uint32 color) {
   int bpp = surf->format->BytesPerPixel;
   Uint8* p = (Uint8 *) surf->pixels + y * surf->pitch + x * bpp;
   *(Uint32 *) p = color;
}

void setBlendColor(SDL_Surface *dst, Sint16 x, Sint16 y, const Color& c) {
//   Uint32 color = SDL_MapRGBA(dst->format, c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
//   setBlendColor(dst, x, y, color, c.getAlpha());

   Uint32 *pixel = (Uint32 *) dst->pixels + y * dst->pitch / 4 + x;
   Uint8 r, g, b, a;
   SDL_GetRGBA(*pixel, dst->format, &r, &g, &b, &a);
   float ca = c.getAlphaF();

   r = (1.f - ca) * r + ca * c.getRed();
   g = (1.f - ca) * g + ca * c.getGreen();
   b = (1.f - ca) * b + ca * c.getBlue();
   a = (1.f - ca) * a + ca * c.getAlpha();

   *pixel = SDL_MapRGBA(dst->format, r, g, b, a);

}

void setBlendColor(SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha) {
   SDL_PixelFormat *format = dst->format;
   Uint32 Rmask, Gmask, Bmask, Amask;
   Uint32 Rshift, Gshift, Bshift, Ashift;
   Uint32 R, G, B, A;

   if (alpha == 255) {
      *((Uint32 *) dst->pixels + y * dst->pitch / 4 + x) = color;
   } else {
      Uint32 *pixel = (Uint32 *) dst->pixels + y * dst->pitch / 4 + x;
      Uint32 dc = *pixel;

      Rmask = format->Rmask;
      Gmask = format->Gmask;
      Bmask = format->Bmask;
      Amask = format->Amask;

      Rshift = format->Rshift;
      Gshift = format->Gshift;
      Bshift = format->Bshift;
      Ashift = format->Ashift;

      R = ((dc & Rmask) + (((((color & Rmask) - (dc & Rmask)) >> Rshift) * alpha >> 8) << Rshift)) & Rmask;
      G = ((dc & Gmask) + (((((color & Gmask) - (dc & Gmask)) >> Gshift) * alpha >> 8) << Gshift)) & Gmask;
      B = ((dc & Bmask) + (((((color & Bmask) - (dc & Bmask)) >> Bshift) * alpha >> 8) << Bshift)) & Bmask;
      *pixel = R | G | B;

      if (Amask!=0) {
         A = ((dc & Amask) + (((((color & Amask) - (dc & Amask)) >> Ashift) * alpha >> 8) << Ashift)) & Amask;
         *pixel |= A;
      }
   }
}
