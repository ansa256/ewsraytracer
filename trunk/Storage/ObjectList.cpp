#include "ObjectList.h"
#include "Math/Maths.h"

typedef list<GeometryObject*>::const_iterator ListIter;

bool ObjectList::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   bool hit = false;
   double tcheck = HUGE_VALUE;
   Vector3D normal;
   Point3D localHitPoint;
   Point3D hitPoint;
   shared_ptr<Material> mat;

   for(ListIter it = objs.begin(); it != objs.end(); ++it) {
      if((*it)->hit(ray, tmin, sr) && (tmin < tcheck)) {
         hit = true;
         tcheck = tmin;
         normal = sr.normal;
         localHitPoint = sr.localHitPoint;
         hitPoint = ray(tmin);
         mat = (*it)->getMaterial();
      }
   }

   if(hit) {
      sr.localHitPoint = localHitPoint;
      sr.hitPoint = hitPoint;
      sr.normal = normal;
      sr.material = mat;
   }

   return hit;
}

bool ObjectList::shadowHit(const Ray& ray, double& tmin) const {
   for(ListIter it = objs.begin(); it != objs.end(); it++) {
      if(!(*it)->ignoreShadow && (*it)->shadowHit(ray, tmin)) {
         return true;
      }
   }

   return false;
}
