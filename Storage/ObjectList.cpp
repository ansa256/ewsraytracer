#include "ObjectList.h"
#include "Math/Maths.h"

bool ObjectList::hit(const Ray& ray, ShadeRecord& sr) const {
   for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
      if((*it)->hit(ray, sr)) {
         return true;
      }
   }

   return false;
}

bool ObjectList::shadowHit(const Ray& ray, double& tHit) const {
   for(GeomIter it = objs.begin(); it != objs.end(); it++) {
      if(!(*it)->ignoreShadow && (*it)->shadowHit(ray, tHit)) {
         return true;
      }
   }

   return false;
}
