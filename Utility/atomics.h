#ifndef raytracer_atomics_h
#define raytracer_atomics_h

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
#endif

typedef volatile int32_t AtomicInt32;

/**
 * If theVal == oldVal, sets theVal = newVal and returns true.
 * Otherwise returns false.
 */
inline bool AtomicCompareAndSet(int32_t oldVal, int32_t newVal, AtomicInt32* theVal) {
#ifdef __APPLE__
   return OSAtomicCompareAndSwap32Barrier(oldVal, newVal, theVal);
#else
#error "Implemnt this"
#endif
}

inline int AtomicAdd(AtomicInt32* val, int32_t delta) {
#ifdef __APPLE__
   return OSAtomicAdd32Barrier(delta, val);
#else
#error "Implement this"
#endif
}

inline float AtomicAdd(volatile float* val, float delta) {
   union bits { float f; int32_t i; };
   bits oldVal, newVal;
   
   do {
      oldVal.f = *val;
      newVal.f = oldVal.f + delta;
   } while(AtomicCompareAndSet(oldVal.i, newVal.i, (AtomicInt32*) val) == false);
   
   return newVal.f;
}

#endif
