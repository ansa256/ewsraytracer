#ifndef _LensFlare_MATH_H_
#define _LensFlare_MATH_H_

template<class T>
T clamp(const T x, const T min, const T max) {
   return x < min ? min : x > max ? max : x;
}

template<class T>
T smootherstep(T edge0, T edge1, T x) {
   // Scale, and clamp x to 0..1 range
   x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
   // Evaluate polynomial
   return x*x*x*(x*(x*6 - 15) + 10);
}

template<class T>
inline T lerp(const float f, const T a, const T b) {
   return a + (b - a) * f;
}

#endif

