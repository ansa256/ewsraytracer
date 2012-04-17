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

void setBlendColor(SDL_Surface *surf, int x, int y, const Color& c) {
   SDL_Rect rect = surf->clip_rect;
   if(x < 0 || x >= rect.w) {
      return;
   }
   if(y < 0 || y >= rect.h) {
      return;
   }
   Uint32 *pixel = (Uint32 *) surf->pixels + y * surf->pitch / 4 + x;
   setBlendColor(surf, pixel, c);
}

void setBlendColor(SDL_Surface* surf, Uint32* pixel, const Color& c) {
   Uint8 r, g, b, a;
   SDL_GetRGBA(*pixel, surf->format, &r, &g, &b, &a);
   float dr = r / 255.f;
   float dg = g / 255.f;
   float db = b / 255.f;
   float da = a / 255.f;

   float sa = c.alpha;

   float ao = sa + da * (1.f - sa);
   float ro = (c.red * sa + dr * da * (1.f - sa)) / ao;
   float go = (c.green * sa + dg * da * (1.f - sa)) / ao;
   float bo = (c.blue * sa + db * da * (1.f - sa)) / ao;

   r = ro * 255;
   g = go * 255;
   b = bo * 255;
   a = ao * 255;

   *pixel = SDL_MapRGBA(surf->format, r, g, b, a);
}

void setBlendColor(SDL_Surface * surf, int x, int y, const Color& c, int weight) {
   int alpha = ((c.getAlpha() * weight) >> 8);
   Color color(c.red, c.green, c.blue, alpha / 255.f);
   setBlendColor(surf, x, y, color);
}

void setAlpha(SDL_Surface* surf, Uint8 alpha) {
   int bpp = surf->format->BytesPerPixel;
   Uint8 r, g, b, a;
   
   for(int y = 0; y < surf->h; y++) {
      for(int x = 0; x < surf->w; x++) {
         Uint8* p = (Uint8 *) surf->pixels + y * surf->pitch + x * bpp;
         SDL_GetRGBA(*(Uint32*) p, surf->format, &r, &g, &b, &a);
         a = (a == 0) ? a : alpha;
         *(Uint32 *) p = SDL_MapRGBA(surf->format, r, g, b, a);
      }
   }
}

void user_error_fn(png_structp ctx, png_const_charp str) {
   printf("libpng error %s\n", str);
}

void user_warning_fn(png_structp ctx, png_const_charp str) {
   printf("libpng warning %s\n", str);
}

int png_colortype_from_surface(SDL_Surface *surface) {
   int colortype = PNG_COLOR_MASK_COLOR; /* grayscale not supported */
   
	if (surface->format->palette)
		colortype |= PNG_COLOR_MASK_PALETTE;
	else if (surface->format->Amask)
		colortype |= PNG_COLOR_MASK_ALPHA;
   
	return colortype;
}

void saveImage(SDL_Surface* surface, const char* fname) {
   png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, user_warning_fn);
   if(png_ptr == NULL) {
      printf("Unable to create png_ptr\n");
      return;
   }
   
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      printf("Unable to create info_ptr\n");
      png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
      return;
   }
   
   // Setup error handling
   if (setjmp(png_jmpbuf(png_ptr))) {
      printf("An error has occured\n");
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return;
   }
   
   // Setup the output code
   FILE* fp = fopen(fname, "wb");
   png_init_io(png_ptr, fp);
   
   int colortype = png_colortype_from_surface(surface);
   png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8,
                colortype, // color type
                PNG_INTERLACE_NONE, // interlace type
                PNG_COMPRESSION_TYPE_DEFAULT, // compression type
                PNG_FILTER_TYPE_DEFAULT); // filter method
   
   png_set_bgr(png_ptr);
   
   /* Writing the image */
   png_write_info(png_ptr, info_ptr);
   png_set_packing(png_ptr);
   
   png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * surface->h);
   for (int i = 0; i < surface->h; i++) {
      row_pointers[i] = (png_bytep)(Uint8 *)surface->pixels + i * surface->pitch;
   }
   
   png_write_image(png_ptr, row_pointers);
   png_write_end(png_ptr, info_ptr);
   
   /* Cleaning out... */
   free(row_pointers);
   png_destroy_write_struct(&png_ptr, &info_ptr);
   fclose(fp);
}
