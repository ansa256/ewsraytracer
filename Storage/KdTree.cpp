#include "KdTree.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include <stack>
#include <limits>

struct NodeS {
   KdNode* node;
   double min, max;

   NodeS(KdNode* n, double _min, double _max) {
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

KdNode::KdNode(vector<int> _idxs) : left(NULL), right(NULL), idxs(_idxs) {
}

KdNode::KdNode(KdNode* l, KdNode* r, int _axis, double _split) :
   left(l),
   right(r),
   idxs(),
   axis(_axis),
   split(_split)
{
}

bool KdNode::isLeaf() const {
   return left == NULL && right == NULL;
}

KdTree::KdTree() :
   Storage(),
   root(NULL),
   edges(NULL),
   maxDepth(0),
   maxObjects(10),
   travCost(1),
   isectCost(80)
{
}

KdTree::~KdTree() {
}

void KdTree::setup() {
   if(maxDepth == 0) {
      maxDepth = int(8 + 1.3f * Log2Int(float(objs.size())));
   }

   edges = new BoundEdge[objs.size() * 2];

   Uint32 start = SDL_GetTicks();

   vector<int> idxs;
   for(unsigned i = 0; i < objs.size(); i++) {
      idxs.push_back(i);
   }

   root = buildTree(0, idxs, bbox);

   Uint32 end = SDL_GetTicks();
   fprintf(stdout, "Build time = %f seconds\n", (end - start) / 1000.0);
   fflush(stdout);
}

void KdTree::findSplit(vector<int>& idxs, const BBox& bounds, int& axis, double& split) {
   double invTotalSA = 1.0 / bounds.surfaceArea();
   double tsplit = numeric_limits<double>::infinity();

   int taxis = bounds.maxExtentAxis();
   int tries = 0;
   bool found = false;

   axis = -1;

   while(!found && tries < 3) {
      int idx = 0;
      for(unsigned i = 0; i < idxs.size(); i++) {
         edges[2 * idx].set(objs[idxs[i]]->bbox.getMin(taxis), true);
         edges[2 * idx + 1].set(objs[idxs[i]]->bbox.getMax(taxis), false);
         idx++;
      }
      sort(&edges[0], &edges[2 * idxs.size()]);

      int nBelow = 0, nAbove = idxs.size();
      double bestCost = numeric_limits<double>::max();

      for(unsigned i = 0; i < 2 * idxs.size(); i++) {
         if(edges[i].type == BoundEdge::END) nAbove--;
         double edget = edges[i].tsplit;
         if(edget > bounds.getMin(taxis) && edget < bounds.getMax(taxis)) {
            int other0 = (taxis + 1) % 3;
            int other1 = (taxis + 2) % 3;

            double belowSA = 2 * (bounds.width(other0) * bounds.width(other1)) +
                                 (edget - bounds.getMin(taxis)) *
                                 (bounds.width(other0) + bounds.width(other1));

            double aboveSA = 2 * (bounds.width(other0) * bounds.width(other1)) +
                                 (bounds.getMax(taxis) - edget) *
                                 (bounds.width(other0) + bounds.width(other1));

            double pBelow = belowSA * invTotalSA;
            double pAbove = aboveSA * invTotalSA;
            double eb = (nBelow == 0 || nAbove == 0) ? 0.2 : 1.0;

            double cost = travCost + isectCost * eb * (pBelow * nBelow + pAbove * nAbove);

            if(cost < bestCost) {
               bestCost = cost;
               tsplit = edget;
               found = bestCost < (80.0 * idxs.size());
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

KdNode* KdTree::buildTree(unsigned depth, vector<int> idxs, const BBox& bounds) {
   if(depth >= maxDepth || idxs.size() < maxObjects) {
      // stop recursion
      return new KdNode(idxs);
   }

   int axis;
   double split;
   findSplit(idxs, bounds, axis, split);

   if(axis == -1) {
      return new KdNode(idxs);
   }

   BBox left = bounds;
   left.setMax(axis, split);
   BBox right = bounds;
   right.setMin(axis, split);

   vector<int> lidxs, ridxs;

   for(unsigned i = 0; i < idxs.size(); i++) {
      if(left.intersects(objs[idxs[i]]->bbox)) {
         lidxs.push_back(idxs[i]);
      }
      if(right.intersects(objs[idxs[i]]->bbox)) {
         ridxs.push_back(idxs[i]);
      }
   }
   return new KdNode(buildTree(depth+1, lidxs, left), buildTree(depth+1, ridxs, right), axis, split);
}

void KdTree::setHash(Hash* hash) {
   if(hash->contains("maxDepth")) {
      maxDepth = hash->getInteger("maxDepth");
   }
   if(hash->contains("maxObjects")) {
      maxObjects = hash->getInteger("maxObjects");
   }
   if(hash->contains("travCost")) {
      travCost = hash->getInteger("travCost");
   }
   if(hash->contains("isectCost")) {
      isectCost = hash->getInteger("isectCost");
   }
}

bool KdTree::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   stack<NodeS> nodeStack;
   nodeStack.push(NodeS(root, 0, HUGE_VALUE));

   double tHit = HUGE_VALUE;

   while(!nodeStack.empty()) {
      KdNode* node = nodeStack.top().node;
      double min = nodeStack.top().min;
      double max = nodeStack.top().max;
      nodeStack.pop();

      while(!node->isLeaf()) {
         double originAxis = ray.origin.get(node->axis);
         double directionAxis = ray.direction.get(node->axis);
         double tsplit = (node->split - originAxis) / directionAxis;
         KdNode *first = node->left, *second = node->right;

         bool belowFirst = (originAxis < node->split) || (originAxis == node->split && directionAxis >= 0);
         if(!belowFirst) {
            first = node->right;
            second = node->left;
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

      if(checkNode(ray, node, tHit, sr) && (tHit < max)) {
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

bool KdTree::checkNode(const Ray& ray, KdNode* node, double& tmin, ShadeRecord& sr) const {
   bool hit = false;
   Vector3D normal;
   Point3D hitPoint;
   Point3D localHitPoint;
   shared_ptr<Material> mat;
   double tcheck = HUGE_VALUE;

   for(unsigned i = 0; i < node->idxs.size(); i++) {
      if(objs[node->idxs[i]]->hit(ray, tmin, sr) && (tmin < tcheck)) {
         tcheck = tmin;
         mat = objs[node->idxs[i]]->getMaterial();
         localHitPoint = sr.localHitPoint;
         normal = sr.normal;
         hitPoint = ray(tmin);
         hit = true;
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

bool KdTree::shadowHit(const Ray& ray, double& tmin) const {
   stack<NodeS> nodeStack;
   nodeStack.push(NodeS(root, 0, HUGE_VALUE));

   double tHit = HUGE_VALUE;

   while(!nodeStack.empty()) {
      KdNode* node = nodeStack.top().node;
      double min = nodeStack.top().min;
      double max = nodeStack.top().max;
      nodeStack.pop();

      while(!node->isLeaf()) {
         double originAxis = ray.origin.get(node->axis);
         double directionAxis = ray.direction.get(node->axis);
         double tsplit = (node->split - originAxis) / directionAxis;
         KdNode *first = node->left, *second = node->right;

         bool belowFirst = (originAxis < node->split) || (originAxis == node->split && directionAxis >= 0);
         if(!belowFirst) {
            first = node->right;
            second = node->left;
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

bool KdTree::checkNodeShadow(const Ray& ray, KdNode* node, double& tmin) const {
   bool hit = false;
   double tcheck = HUGE_VALUE;

   for(unsigned i = 0; i < node->idxs.size(); i++) {
      if(!objs[node->idxs[i]]->ignoreShadow && objs[node->idxs[i]]->shadowHit(ray, tmin) && (tmin < tcheck)) {
         tcheck = tmin;
         hit = true;
      }
   }

   return hit;
}
