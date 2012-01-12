#include "Color.h"
#include <algorithm>
#include <math.h>

using namespace std;

const Color BLACK;

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

Color::Color(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();

   if(a->size() > 3) {
      alpha = a->at(3)->getDouble();
   }
   else {
      alpha = 1.0;
   }
}

void Color::set(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
}

void Color::set(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();
   alpha = 1.0;
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
