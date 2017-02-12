#ifndef OCTREE_H
#define OCTREE_H

#include <array>
#include <list>
#include <memory>
#include <queue>

#include "glm/glm.hpp"

#include "GameObject.h"

/* 
 * Spatial data structure used to store static objects in the world
 *
 * Design influenced and informed by: 
 */
class OctreeNode {
public:

   //
   OctreeNode();

   //
   OctreeNode(OctreeNode* parent, glm::vec3& min, glm::vec3& max);

   //
   ~OctreeNode();

   //
   void addObject(std::shared_ptr<GameObject> obj);

   //
   void buildTree();

   //
   std::shared_ptr<GameObject> checkIntersection(std::shared_ptr<GameObject> objToCheck);

private:

   // The children whose parent is this node
   std::vector<OctreeNode> children_;

   // The representation of the region of space contained within the octree
   BoundingBox enclosingRegion_;

   // The objects within the current Octree node enclosure
   std::vector<std::shared_ptr<GameObject>> objsEnclosed;

   // Any objects that have yet to be added to the Octree (should only have objects within the parent node)
   std::list<std::shared_ptr<GameObject>> objsNotInTree;

   // The parent of this node
   OctreeNode* parent_;

   //
   void buildTreeNode();

   //
   void createEnclosingRegionForRoot();

   //
   bool contains(const std::shared_ptr<GameObject> obj);

};

#endif
