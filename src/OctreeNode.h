#ifndef OCTREE_H
#define OCTREE_H

#include <array>
#include <list>
#include <memory>
#include <vector>
#include <queue>

#include "glm/glm.hpp"

#include "GameObject.h"
#include "ViewFrustum.h"

/* 
 * Spatial data structure used to store static objects in the world
 *
 * Design influenced and informed by: https://www.gamedev.net/resources/_/technical/game-programming/introduction-to-octrees-r3529
 */
class OctreeNode {
public:

   // Constructs a root Octree node
   OctreeNode();

   // Constructs a child Octree node with the given parent and size
   OctreeNode(OctreeNode* parent, glm::vec3& min, glm::vec3& max);

   ~OctreeNode();

   // Adds an object to the list of objects to be inserted into the tree when built
   void addObject(std::shared_ptr<GameObject> obj);

   // Builds a tree with the current node as the root
   void buildTree();

   // Clears all internal lists/vectors of GameObjects currently in the tree
   void clearTree();

   // Returns an object that intersects with the past object. Returns |nullptr| if no such object exists
   std::vector<std::shared_ptr<GameObject>> checkIntersection(std::shared_ptr<GameObject> objToCheck);

   // Draws all objects within the given view frustum
   void cullAndDrawObjs(ViewFrustum& viewFrustum, std::shared_ptr<MatrixStack> P,
	   std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V);

private:

   // The children whose parent is this node
   std::vector<OctreeNode> children_;

   // The representation of the region of space contained within the octree
   std::shared_ptr<BoundingBox> enclosingRegion_;

   // The objects within the current Octree node enclosure
   std::vector<std::shared_ptr<GameObject>> objsEnclosed_;

   // Any objects that have yet to be added to the Octree (should only have objects within the parent node)
   std::list<std::shared_ptr<GameObject>> objsNotInTree_;

   // The parent of this node
   OctreeNode* parent_;

   // Builds the tree node 
   void buildTreeNode();

   // Finds the min and max of all the objects to be added to the tree and creates a BoudingBox based
   // on the smallest min and biggest max
   void createEnclosingRegionForRoot();

   // Checks if the object is contained within the |enclosingRegion_| of the node
   bool contains(const std::shared_ptr<GameObject> obj);

};

#endif
