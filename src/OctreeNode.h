#ifndef OCTREE_H
#define OCTREE_H

/* 
 * Spatial data structure used to store static objects in the world
 *
 * Design influenced and informed by: 
 */
class OctreeNode {
public:

   OctreeNode(const BoundingBox& enclosingRegion);

   OctreeNode(const glm::vec3& min, const glm::vec3& max);

   OctreeNode(std::vector<std::shared_ptr<GameObject>>& objs);

   ~OctreeNode();

private:

   // The children whose parent is this node
   std::array<OctreeNode, 8> children_;

   // The representation of the region of space contained within the octree
   BoundingBox enclosingRegion_;

   // The objects within the current Octree node enclosure
   std::vector<std::shared_ptr<GameObject>> objsEnclosed;

   // Any objects that have yet to be added to the Octree (should only have objects within the parent node)
   std::vector<std::shared_ptr<GameObject>> objsNotInTree;

   // The parent of this node
   std::shared_ptr<OctreeNode> parent_;

};

#endif
