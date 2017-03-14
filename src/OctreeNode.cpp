#include "OctreeNode.h"

OctreeNode::OctreeNode() {
   parent_ = NULL;
}

OctreeNode::OctreeNode(OctreeNode* parent,  glm::vec3& min, glm::vec3& max) {
   parent_ = parent;
   enclosingRegion_ = std::make_shared<BoundingBox>(min, max);
}

OctreeNode::~OctreeNode() {

}

void OctreeNode::addObject(std::shared_ptr<GameObject> obj) {
   objsNotInTree_.push_front(obj);
}

void OctreeNode::createEnclosingRegionForRoot() {
   glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
   glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

   // Find the smallest min and max from every object that will be in the tree
   for (std::shared_ptr<GameObject> obj : objsNotInTree_) {
      if (obj->getBoundingBox() != nullptr) {
         glm::vec3& objMin = obj->getBoundingBox()->min_;
         glm::vec3& objMax = obj->getBoundingBox()->max_;

         min.x = objMin.x < min.x ? objMin.x : min.x;
         min.y = objMin.y < min.y ? objMin.y : min.y;
         min.z = objMin.z < min.z ? objMin.z : min.z;

		 // Small heuristic to better match GG world construction (no objects that are in upper 4 octants exclusively by default)
         max.x = objMax.x + (objMax.x * 4) > max.x ? objMax.x + (objMax.x * 4) : max.x;
         max.y = objMax.y + (objMax.y * 4) > max.y ? objMax.y + (objMax.y * 4) : max.y;
         max.z = objMax.z + (objMax.z * 4) > max.z ? objMax.z + (objMax.z * 4) : max.z;
      }
   }

   enclosingRegion_ = std::make_shared<BoundingBox>(min, max);
}

bool OctreeNode::contains(const std::shared_ptr<GameObject> obj) {
   std::shared_ptr<BoundingBox> objBoundBox = obj->getBoundingBox();

   // Object has no bounding box
   if (objBoundBox == nullptr) {
      return false;
   }

   // Make sure each point of the object's bounding box is within the node's enclosing region
   if (enclosingRegion_->min_.x <= objBoundBox->min_.x
	   && enclosingRegion_->min_.y <= objBoundBox->min_.y
	   && enclosingRegion_->min_.z <= objBoundBox->min_.z
	   && enclosingRegion_->max_.x >= objBoundBox->max_.x
	   && enclosingRegion_->max_.y >= objBoundBox->max_.y
	   && enclosingRegion_->max_.z >= objBoundBox->max_.z) {
      return true;
   }

   return false;
}

void OctreeNode::buildTree() {
   clearTree();
   createEnclosingRegionForRoot();
   buildTreeNode();
}

void OctreeNode::clearTree() {
   for (OctreeNode& child : children_) {
      child.clearTree();
   }

   children_.clear();
   objsEnclosed_.clear();
}

std::vector<std::shared_ptr<GameObject>> OctreeNode::checkIntersection(std::shared_ptr<GameObject> objToCheck) {
   std::vector<std::shared_ptr<GameObject>> hitObjs;

   // Check for a child that contains the object and recursively call it's |checkIntersection| method
   for (OctreeNode& child : children_) {
	  if (objToCheck->getBoundingBox()->checkIntersection(child.enclosingRegion_)) {
         std::vector<std::shared_ptr<GameObject>> childHitObjs = child.checkIntersection(objToCheck);
         hitObjs.insert(hitObjs.end(), 
          std::make_move_iterator(childHitObjs.begin()),
          std::make_move_iterator(childHitObjs.end()));
      }
   }

   // Check all the objects belonging to this node (since the object must be contained within)
   for (std::shared_ptr<GameObject> objInTree : objsEnclosed_) {
      if (objToCheck->checkIntersection(objInTree)) {
         hitObjs.push_back(objInTree);
      }
   }

   return hitObjs;
}

void OctreeNode::cullAndDrawObjs(ViewFrustum& viewFrustum, std::shared_ptr<MatrixStack> P,
	std::shared_ptr<MatrixStack> M, std::shared_ptr<MatrixStack> V) {

	// Check for a child that contains the object and recursively call it's |checkIntersection| method
	for (OctreeNode& child : children_) {
		if (child.enclosingRegion_ != nullptr) {
			if (!viewFrustum.cull(child.enclosingRegion_)) {
				//std::cout << "HERE" << std::endl;
				child.cullAndDrawObjs(viewFrustum, P, M, V);
			}
		}
	}

	// Check all the objects belonging to this node (since the object must be contained within)
	for (std::shared_ptr<GameObject> objInTree : objsEnclosed_) {
		std::shared_ptr<BoundingBox> boundBox = objInTree->getBoundingBox();
		if (boundBox != nullptr) {
			if (!viewFrustum.cull(boundBox)) {
				objInTree->draw(P, M, V);
			}
		}
		else {
			objInTree->draw(P, M, V);
		}
	}
}
   
void OctreeNode::buildTreeNode() {
   glm::vec3& regionMin = enclosingRegion_->min_;
   glm::vec3& regionMax = enclosingRegion_->max_;

   // Calculate the center of the current region
   glm::vec3 regionDim(regionMax - regionMin);
   glm::vec3 halfRegionDim(regionDim / 2.0f);
   glm::vec3 centerOfRegion(regionMin + halfRegionDim);

   // If region is a minimum  size, stop dividing and put everything left in here
   if (std::abs(regionDim.x) <= 1.0f && std::abs(regionDim.y) <= 1.0f && std::abs(regionDim.z) <= 1.0f) {
      for (std::shared_ptr<GameObject> obj : objsNotInTree_) {
         objsEnclosed_.push_back(obj);
      }
      objsNotInTree_.clear();
      
      return;
   }
   
   std::cout << "HERE" << std::endl;
   std::cout << "Region Min: " << regionMin.x << " " << regionMin.y << " " << regionMin.z << std::endl;
   std::cout << "Region Max: " << regionMax.x << " " << regionMax.y << " " << regionMax.z << std::endl;

   glm::vec3 childMin;
   glm::vec3 childMax;

   // Build each octant child of the current node
   childMin = regionMin;
   childMax = centerOfRegion;
   children_.emplace_back(this, childMin, childMax);

   childMin = glm::vec3(centerOfRegion.x, regionMin.y, regionMin.z);
   childMax = glm::vec3(regionMax.x, centerOfRegion.y, centerOfRegion.z);
   children_.emplace_back(this, childMin, childMax);

   childMin = glm::vec3(centerOfRegion.x, regionMin.y, centerOfRegion.z);
   childMax = glm::vec3(regionMax.x, centerOfRegion.y, regionMax.z);
   children_.emplace_back(this, childMin, childMax);

   childMin = glm::vec3(regionMin.x, regionMin.y, centerOfRegion.z);
   childMax = glm::vec3(centerOfRegion.x, centerOfRegion.y, regionMax.z);
   children_.emplace_back(this, childMin, childMax);

   childMin = glm::vec3(regionMin.x, centerOfRegion.y, regionMin.z);
   childMax = glm::vec3(centerOfRegion.x, regionMax.y, centerOfRegion.z);
   children_.emplace_back(this, childMin, childMax);

   childMin = glm::vec3(centerOfRegion.x, centerOfRegion.y, regionMin.z);
   childMax = glm::vec3(regionMax.x, regionMax.y, centerOfRegion.z);
   children_.emplace_back(this, childMin, childMax);

   childMin = centerOfRegion;
   childMax = regionMax;
   children_.emplace_back(this, childMin, childMax);

   childMin = glm::vec3(regionMin.x, centerOfRegion.y, centerOfRegion.z);
   childMax = glm::vec3(centerOfRegion.x, regionMax.y, centerOfRegion.z);
   children_.emplace_back(this, childMin, childMax);

   // Enqueue any objects that are going to be handled by a child
   std::queue<std::shared_ptr<GameObject>> objsRemoved;

   // Find all objects that fit perfectly in the child node
   for (std::shared_ptr<GameObject> obj : objsNotInTree_) {
      for (OctreeNode& child : children_) {
         if (child.contains(obj)) {

            // If the child node perfectly contains the object, add it to it's list
            // and set it for removal from the parent list
            child.objsNotInTree_.push_front(obj);
            objsRemoved.push(obj);
            break;
         }
      }
   }

   // Remove objects now handled by a child node so that they are not added to this node's list below
   while (!objsRemoved.empty()) {
      std::shared_ptr<GameObject> objToRemove = objsRemoved.front();
      objsRemoved.pop();
      objsNotInTree_.remove(objToRemove);
   }

   std::cout << "objsNotInTree: " << objsNotInTree_.size() << std::endl;

   // Move all objects that don't fit perfectly into a child node into the list for this node
   // as this is the smallest enclosing space it fits perfectly into
   for (std::shared_ptr<GameObject> obj : objsNotInTree_) {
      objsEnclosed_.push_back(obj);
   }

   // All objects are now either in the tree or in a child's list
   objsNotInTree_.clear();

   // Check children for remaining objects to add to the tree
   for (OctreeNode& child : children_) {
      if (child.objsNotInTree_.size() > 0) {

         // Child has objects to further sort so we keep going
         child.buildTreeNode();
      }
   }
}
