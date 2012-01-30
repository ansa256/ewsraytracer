#include "Tracer.h"
#include "Math/Maths.h"
#include "Geometry/GeometryManager.h"
#include "Storage/Storage.h"
#include "Materials/Material.h"

Tracer::Tracer() : bgColor(), texture(NULL), maxDepth(10) {
}

Tracer::~Tracer() {
   if(texture != NULL) {
      delete texture;
   }
}

Color Tracer::traceRay(const Ray& ray, const int depth) {
   if(depth > maxDepth) {
      return BLACK;
   }
   
   ShadeRecord sr;
   ray.tHit = HUGE_VALUE;
   
   if(GeometryManager::instance().getStorage()->hit(ray, sr)) {
      sr.depth = depth;
      Color c = sr.material->shade(sr, ray);
      
      if(c.getAlphaF() < 1.0) {
         Ray newRay(sr.hitPoint, ray.direction);
         Color newColor = traceRay(newRay, depth);
         return c * c.alpha + newColor * (1.0 - c.alpha);
      }
      return c;
   }
   else {
      if(texture != NULL) {
         sr.localHitPoint.set(ray.direction.x, ray.direction.y, ray.direction.z);
         return texture->getColor(sr);
      }
      return bgColor;
   }
}
