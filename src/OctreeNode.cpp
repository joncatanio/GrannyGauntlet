#include "OctreeNode.h"

OctreeNode::OctreeNode(const BoundingBox& enclosingRegion) {
   enclosingRegion_ = enclosingRegion;
}

OctreeNode::OctreeNode(const glm::vec3& min, const glm::vec3& max) {
   enclosingRegion_ = BoundingBox(min, max);
}

OctreeNode::OctreeNode(std::vector<std::shared_ptr<GameObject>>& objs) {
   // find min and max of entire list
   // create enclosing region
   // build tree
   enclosingRegion_ = enclosingRegion;
}

OctreeNode::~OctreeNode() {

}

bool OctreeNode::contains(const std::shared_ptr<GameObject> obj) {
   BoundingBox* objBoundBox = obj->getBoundingBox();

   if (enclosingRegion_.min_ < objBoundBox.min_ 
    && enclosingRegion_.max > objBoundBox.max_) {
      return true;
   }

   return false;
}
   
OctreeNode::build(std::vector<std::shared_ptr<GameObject>>& objs, std::vector<std::shared_ptr<GameObject>>::iterator start, std::vector<std::shared_ptr<GameObject>>::iterator end) {
   if (start == end) {
      return;
   }

   glm::vec3& regionMin = enclosingRegion_.min_;
   glm::vec3& regionMax = enclosingRegion_.max_;

   // Calculate the center of the current region
   glm::vec3 regionDim(regionMax - regionMin);
   glm::vec3 halfRegionDim(regionDim / 2.0f);
   glm::vec3 centerOfRegion(regionMin + halfRegionDim);

   glm::vec3 childMin;
   glm::vec3 childMax;

   childMin = enclosingRegion_.min;
   childMax = centerOfRegion;
   children_[0](childMin, childMax);

   childMin = glm::vec3(centerOfRegion.x, regionMin.y, regionMin.z);
   childMax = glm::vec3(regionMax.x, centerOfRegion.y, centerOfRegion.z);
   children_[1](childMin, childMax);

   childMin = glm::vec3(centerOfRegion.x, regionMin.y, centerOfRegion.z);
   childMax = glm::vec3(regionMax.x, centerOfRegion.y, regionMax.z);
   children_[2](childMin, childMax);

   childMin = glm::vec3(regionMin.x, regionMin.y, centerOfRegion.z);
   childMax = glm::vec3(centerOfRegion.x, centerOfRegion.y, regionMax.z);
   children_[3](childMin, childMax);

   childMin = glm::vec3(regionMin.x, centerOfRegion.y, regionMin.z);
   childMax = glm::vec3(centerOfRegion.x, regionMax.y, centerOfRegion.z);
   children_[4](childMin, childMax);

   childMin = glm::vec3(centerOfRegion.x, centerOfRegion.y, regionMin.z);
   childMax = glm::vec3(regionMax.x, regionMax.y, centerOfRegion.z);
   children_[5](childMin, childMax);

   childMin = centerOfRegion;
   childMax = regionMax;
   children_[6](childMin, childMax);

   childMin = glm::vec3(regionMin.x, centerOfRegion.y, centerOfRegion.z);
   childMax = glm::vec3(centerOfRegion.x, regionMax.y, centerOfRegion.z);
   children_[7](childMin, childMax);

   for (std::shared_ptr<GameObject> obj : objs) {
      for (OctreeNode child : children) {
         if (child.contains(obj)) {
            child. 
         }
      }
   }
}
