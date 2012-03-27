#include "Color.h"
#include <algorithm>
#include <math.h>

using namespace std;

float epsilon = 1.0 * pow(10, -6);

Color::Color(float _r, float _g, float _b, float _a) {
   red = _r;
   green = _g;
   blue = _b;
   alpha = _a;
}

Color::Color(const Color& c) {
   red = c.red;
   green = c.green;
   blue = c.blue;
   alpha = c.alpha;
}

void Color::set(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
}

Color& Color::operator= (const Color& p) {
   red = p.red;
   green = p.green;
   blue = p.blue;
   alpha = p.alpha;
   return *this;
}

Color& Color::operator+= (const Color& c) {
   red += c.red;
   green += c.green;
   blue += c.blue;
   return *this;
}

Color& Color::operator+=(const float a) {
   red += a;
   green += a;
   blue += a;
   return *this;
}

Color& Color::operator/= (const float a) {
   red /= a;
   green /= a;
   blue /= a;
   return *this;
}

Color& Color::operator*= (const float a) {
   red *= a;
   green *= a;
   blue *= a;
   return *this;
}

bool Color::operator== (const Color& c) const {
   if(fabs(red - c.red) > epsilon) {
      return false;
   }
   if(fabs(green - c.green) > epsilon) {
      return false;
   }
   if(fabs(blue - c.blue) > epsilon) {
      return false;
   }
   return true;
}

void Color::normalize() {
   red = min(red, 1.0f);
   green = min(green, 1.0f);
   blue = min(blue, 1.0f);
}
