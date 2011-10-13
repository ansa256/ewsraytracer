#include "KdTree.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include <stack>
#include <limits>

struct NodeS {
   int node;
   double min, max;

   NodeS(int n, double _min, double _max) {
      node = n;
      min = _min;
      max = _max;
   }
};

bool BoundEdge::operator< (const BoundEdge& e) const {
   if(tsplit == e.tsplit) {
      return type < e.type;
   }
   return tsplit < e.tsplit;
}

void KdNode::initLeaf(uint32_t* _idxs, int np) {
   flags = 3;
   nPrims |= (np << 2);
   // Store primitive ids for leaf node
   if (np > 0) {
      idxs = new uint32_t[np];
      for (int i = 0; i < np; ++i) {
         idxs[i] = _idxs[i];
      }
   }
}

void KdNode::initInterior(uint32_t axis, uint32_t ac, float s) {
   split = s;
   flags = axis;
   aboveChild |= (ac << 2);
}

KdTree::KdTree() :
   Storage(),
   nodes(NULL),
   edges(NULL),
   nAllocedNodes(0),
   nextFreeNode(0),
   maxDepth(0),
   travCost(1),
   isectCost(80)
{
}

KdTree::~KdTree() {
   delete[] nodes;
}

void KdTree::setup() {
   if(maxDepth == 0) {
      maxDepth = int(8 + 1.3f * Log2Int(float(objs.size())));
   }

   edges = new BoundEdge[objs.size() * 2];

   Uint32 start = SDL_GetTicks();

   uint32_t* idxs = new uint32_t[objs.size()];
   for(unsigned i = 0; i < objs.size(); i++) {
      idxs[i] = i;
   }

   uint32_t* lidxs = new uint32_t[objs.size()];
   uint32_t* ridxs = new uint32_t[(maxDepth+1) * objs.size()];

   buildTree(0, 0, idxs, objs.size(), bbox, lidxs, ridxs);

   Uint32 end = SDL_GetTicks();
   fprintf(stdout, "Build time = %f seconds\n", (end - start) / 1000.0);
   fflush(stdout);

   delete[] edges;
   delete[] lidxs;
   delete[] ridxs;
}

void KdTree::findSplit(uint32_t* idxs, int np, const BBox& bounds, int& axis, double& split) {
   double invTotalSA = 1.0 / bounds.surfaceArea();
   double tsplit = numeric_limits<double>::infinity();

   int taxis = bounds.maxExtentAxis();
   int tries = 0;
   bool found = false;

   axis = -1;

   while(!found && tries < 3) {
      int idx = 0;
      for(int i = 0; i < np; i++) {
         edges[2 * idx].set(objs[idxs[i]]->bbox.getMin(taxis), true);
         edges[2 * idx + 1].set(objs[idxs[i]]->bbox.getMax(taxis), false);
         idx++;
      }
      sort(&edges[0], &edges[2 * np]);

      int nBelow = 0, nAbove = np;
      double bestCost = numeric_limits<double>::max();

      int other0 = (taxis + 1) % 3;
      int other1 = (taxis + 2) % 3;
      double bw1 = bounds.width(other0) * bounds.width(other1);
      double bw2 = bounds.width(other0) + bounds.width(other1);

      double axisMin = bounds.getMin(taxis);
      double axisMax = bounds.getMax(taxis);

      for(int i = 0; i < 2 * np; i++) {
         if(edges[i].type == BoundEdge::END) nAbove--;
         double edget = edges[i].tsplit;
         if(edget > axisMin && edget < axisMax) {
            double belowSA = 2 * bw1 + (edget - axisMin) * bw2;
            double aboveSA = 2 * bw1 + (axisMax - edget) * bw2;

            double pBelow = belowSA * invTotalSA;
            double pAbove = aboveSA * invTotalSA;
            double eb = (nBelow == 0 || nAbove == 0) ? 0.2 : 1.0;

            double cost = travCost + isectCost * eb * (pBelow * nBelow + pAbove * nAbove);

            if(cost < bestCost) {
               bestCost = cost;
               tsplit = edget;
               found = bestCost < (isectCost * np);
            }
         }
         if(edges[i].type == BoundEdge::START) nBelow++;
      }

      if(!found) {
         tries++;
         taxis = (taxis + 1) % 3;
      }
   }
   if(found) {
      axis = taxis;
      split = tsplit;
   }
}

void KdTree::buildTree(int nodeNum, unsigned depth, uint32_t* idxs, int nPrimitives, const BBox& bounds, uint32_t* lidxs, uint32_t* ridxs) {
   assert(nodeNum == nextFreeNode);

   // Get next free node from _nodes_ array
   if (nextFreeNode == nAllocedNodes) {
      int nAlloc = max(2 * nAllocedNodes, 512);
      KdNode *n = new KdNode[nAlloc];
      if (nAllocedNodes > 0) {
         memcpy(n, nodes, nAllocedNodes * sizeof(KdNode));
         delete[] nodes;
      }
      nodes = n;
      nAllocedNodes = nAlloc;
   }
   ++nextFreeNode;

   if(depth >= maxDepth) {
      // stop recursion
      nodes[nodeNum].initLeaf(idxs, nPrimitives);
      return;
   }

   int axis;
   double split;
   findSplit(idxs, nPrimitives, bounds, axis, split);

   if(axis == -1) {
      nodes[nodeNum].initLeaf(idxs, nPrimitives);
      return;
   }

   BBox left = bounds;
   left.setMax(axis, split);
   BBox right = bounds;
   right.setMin(axis, split);

   int n0 = 0, n1 = 0;

   for(int i = 0; i < nPrimitives; i++) {
      if(left.intersects(objs[idxs[i]]->bbox)) {
         lidxs[n0++] = idxs[i];
      }
      if(right.intersects(objs[idxs[i]]->bbox)) {
         ridxs[n1++] = idxs[i];
      }
   }

   buildTree(nodeNum+1, depth+1, lidxs, n0, left, lidxs, ridxs + nPrimitives);
   nodes[nodeNum].initInterior(axis, nextFreeNode, split);
   buildTree(nextFreeNode, depth+1, ridxs, n1, right, lidxs, ridxs + nPrimitives);
//   return new KdNode(buildTree(depth+1, lidxs, left), buildTree(depth+1, ridxs, right), axis, split);
}

void KdTree::setHash(Hash* hash) {
   if(hash->contains("maxDepth")) {
      maxDepth = hash->getInteger("maxDepth");
   }
   if(hash->contains("travCost")) {
      travCost = hash->getInteger("travCost");
   }
   if(hash->contains("isectCost")) {
      isectCost = hash->getInteger("isectCost");
   }
}

bool KdTree::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   boost::dynamic_bitset<> checked(objs.size());

   stack<NodeS> nodeStack;
   nodeStack.push(NodeS(0, 0, HUGE_VALUE));

   double tHit = HUGE_VALUE;

   while(!nodeStack.empty()) {
      int node = nodeStack.top().node;
      double min = nodeStack.top().min;
      double max = nodeStack.top().max;
      nodeStack.pop();

      while(!nodes[node].isLeaf()) {
         double originAxis = ray.origin.get(nodes[node].getAxis());
         double directionAxis = ray.direction.get(nodes[node].getAxis());
         double tsplit = (nodes[node].getSplit() - originAxis) / directionAxis;

         int first = node + 1, second = nodes[node].getAboveChild();
         bool belowFirst = (originAxis < nodes[node].getSplit()) || (originAxis == nodes[node].getSplit() && directionAxis >= 0);
         if(!belowFirst) {
            swap(first, second);
         }

         if(tsplit > max || tsplit < 0) {
            node = first;
         }
         else if(tsplit < min) {
            node = second;
         }
         else {
            nodeStack.push(NodeS(second, tsplit, max));
            node = first;
            max = tsplit;
         }
      }

      if(checkNode(ray, node, tHit, sr, checked) && (tHit < max)) {
         tmin = tHit;
         return true;
      }
   }

   return false;
/*
stack.push(root,sceneMin,sceneMax)
tHit=infinity
while (not stack.empty()):
   (node,tMin,tMax)=stack.pop()
   while (not node.isLeaf()):
      a = node.axis
      tSplit = ( node.value - ray.origin[a] ) / ray.direction[a]
      (first, sec) = order(ray.direction[a], node.left, node.right)
      if( tSplit >= tMax or tSplit < 0)
         node=first
      else if( tSplit <= tMin)
         node=second
      else
         stack.push( sec, tSplit, tMax)
         node=first
         tMax=tSplit
   for tri in node.triangles():
      tHit=min(tHit,tri.Intersect(ray))
      if tHit<tMax:
         return tHit //early exit
return tHitt
*/
}

bool KdTree::checkNode(const Ray& ray, int node, double& tmin, ShadeRecord& sr, boost::dynamic_bitset<>& checked) const {
   bool hit = false;
   Vector3D normal;
   Point3D hitPoint;
   Point3D localHitPoint;
   shared_ptr<Material> mat;
   double tcheck = HUGE_VALUE;

   for(uint32_t i = 0; i < nodes[node].nPrimitives(); i++) {
      if(!checked[nodes[node].idxs[i]]) {
         GeometryObject* obj = objs[nodes[node].idxs[i]];
         if(obj->hit(ray, tmin, sr) && (tmin < tcheck)) {
            tcheck = tmin;
            mat = obj->getMaterial();
            localHitPoint = sr.localHitPoint;
            normal = sr.normal;
            hitPoint = ray(tmin);
            hit = true;
         }
         else {
            checked[nodes[node].idxs[i]] = 1;
         }
      }
   }

   if(hit) {
      sr.localHitPoint = localHitPoint;
      sr.hitPoint = hitPoint;
      sr.normal = normal;
      sr.material = material = mat;
   }
   return hit;
}

bool KdTree::shadowHit(const Ray& ray, double& tmin) const {
   stack<NodeS> nodeStack;
   nodeStack.push(NodeS(0, 0, HUGE_VALUE));

   double tHit = HUGE_VALUE;

   while(!nodeStack.empty()) {
      int node = nodeStack.top().node;
      double min = nodeStack.top().min;
      double max = nodeStack.top().max;
      nodeStack.pop();

      while(!nodes[node].isLeaf()) {
         double originAxis = ray.origin.get(nodes[node].getAxis());
         double directionAxis = ray.direction.get(nodes[node].getAxis());
         double tsplit = (nodes[node].getSplit() - originAxis) / directionAxis;
         int first = node + 1, second = nodes[node].getAboveChild();

         bool belowFirst = (originAxis < nodes[node].getSplit()) || (originAxis == nodes[node].getSplit() && directionAxis >= 0);
         if(!belowFirst) {
            swap(first, second);
         }

         if(tsplit > max || tsplit < 0) {
            node = first;
         }
         else if(tsplit <= min) {
            node = second;
         }
         else {
            nodeStack.push(NodeS(second, tsplit, max));
            node = first;
            max = tsplit;
         }
      }
      if(checkNodeShadow(ray, node, tHit) && (tHit < max)) {
         tmin = tHit;
         return true;
      }
   }
   return false;
}

bool KdTree::checkNodeShadow(const Ray& ray, int node, double& tmin) const {
   bool hit = false;
   double tcheck = HUGE_VALUE;

   for(uint32_t i = 0; i < nodes[node].nPrimitives(); i++) {
      GeometryObject* obj = objs[nodes[node].idxs[i]];
      if(!obj->ignoreShadow && obj->shadowHit(ray, tmin) && (tmin < tcheck)) {
         tcheck = tmin;
         hit = true;
      }
   }

   return hit;
}