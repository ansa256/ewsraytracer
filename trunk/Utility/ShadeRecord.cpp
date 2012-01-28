#include "ShadeRecord.h"

ShadeRecord::ShadeRecord() :
   normal(),
   hitPoint(),
   localHitPoint(),
   material(NULL),
   dpdu(),
   dpdv(),
   tu(0),
   tv(0),
   depth(0),
   samplePoint(),
   lightNormal(),
   wi()
{
}

ShadeRecord::ShadeRecord(const ShadeRecord& other) {
   normal = other.normal;
   hitPoint = other.hitPoint;
   localHitPoint = other.localHitPoint;
   material = other.material;
   dpdu = other.dpdu;
   dpdv = other.dpdv;
   tu = other.tu;
   tv = other.tv;
   depth = other.depth;
   samplePoint = other.samplePoint;
   lightNormal = other.lightNormal;
   wi = other.wi;
}
