#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL/SDL.h>

class Color {

public:
   Color(float _r = 0, float _g = 0, float _b = 0, float _a = 1);
   Color(const Color& c);

   void set(float r, float b, float g, float a);

   Color& operator=(const Color& c);
   Color& operator+=(const Color& c);
   Color& operator+=(const float a);
   Color& operator/= (const float a);
   Color& operator*= (const float a);
   bool operator== (const Color& c) const;
   Color operator*(const float a) const;
   Color operator*(const Color& c) const;
   Color operator/(const float a) const;
   Color operator+(const Color& c) const;
   Color operator-(const Color& c) const;

   void normalize();

   inline Uint8 getRed() const { return (Uint8)(red * 255); }
   inline Uint8 getGreen() const { return (Uint8)(green * 255); }
   inline Uint8 getBlue() const { return (Uint8)(blue * 255); }
   inline Uint8 getAlpha() const { return (Uint8)(alpha * 255); }
   float getAlphaF() const { return alpha; }

   float red, green, blue, alpha;
};

inline Color Color::operator*(const float a) const {
   return Color(red * a, green * a, blue * a, alpha * a);
}

inline Color Color::operator*(const Color& c) const {
   return Color(red * c.red, green * c.green, blue * c.blue, alpha * c.alpha);
}

inline Color Color::operator/(const float a) const {
   return Color(red / a, green / a, blue / a, alpha / a);
}

inline Color Color::operator+(const Color& c) const {
   return Color(red + c.red, green + c.green, blue + c.blue, alpha + c.alpha);
}

inline Color Color::operator-(const Color& c) const {
   return Color(red - c.red, green - c.green, blue - c.blue, alpha - c.alpha);
}

extern const Color BLACK;

#endif
