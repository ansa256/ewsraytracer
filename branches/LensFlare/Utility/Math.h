#ifndef LensFlare_Math_h
#define LensFlare_Math_h

template<class T>
inline T lerp(const float f, const T a, const T b) {
   return a + (b - a) * f;
}

#endif
