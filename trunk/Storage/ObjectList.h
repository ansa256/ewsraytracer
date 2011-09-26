#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "Storage.h"

class ObjectList : public Storage {

public:
   ObjectList() {}
   virtual ~ObjectList() {}

   virtual void setup() {}
   virtual void setHash(Hash* hash) {}
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
};

#endif // OBJECTLIST_H
