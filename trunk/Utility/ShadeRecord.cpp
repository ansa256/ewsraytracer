#include "ShadeRecord.h"

ShadeRecord::ShadeRecord() :
   normal(),
   hitPoint(),
   localHitPoint(),
   material(),
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
