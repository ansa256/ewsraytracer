#include "ShadeRecord.h"

ShadeRecord::ShadeRecord() :
   normal(),
   hitPoint(),
   localHitPoint(),
   material(NULL),
   samplePoint(),
   tu(0),
   tv(0),
   depth(0)
{
}

ShadeRecord::ShadeRecord(const ShadeRecord& other) {
   normal = other.normal;
   hitPoint = other.hitPoint;
   localHitPoint = other.localHitPoint;
   material = other.material;
   samplePoint = other.samplePoint;
   tu = other.tu;
   tv = other.tv;
   depth = other.depth;
}
