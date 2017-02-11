#include "ViewFrustum.h"
#include "BoundingBox.h"

using namespace glm;

ViewFrustum::ViewFrustum() {}

ViewFrustum::~ViewFrustum() {}

void ViewFrustum::extractPlanes(glm::mat4 P, glm::mat4 V) {
	/* Composite Matrix
    * Access: mat4[col][row]
    */
	mat4 comp = P*V;
	vec3 normal;     // Normal of a plane
	float length;    // Length of the normal, used for normalization

   // Left plane
	planes[0].x = comp[0][3] + comp[0][0];
	planes[0].y = comp[1][3] + comp[1][0];
	planes[0].z = comp[2][3] + comp[2][0];
	planes[0].w = comp[3][3] + comp[3][0];
   normal = vec3(planes[0].x, planes[0].y, planes[0].z);
   length = glm::length(normal);
	planes[0] = planes[0] / length;

   // Right plane
	planes[1].x = comp[0][3] - comp[0][0];
	planes[1].y = comp[1][3] - comp[1][0];
	planes[1].z = comp[2][3] - comp[2][0];
	planes[1].w = comp[3][3] - comp[3][0];
   normal = vec3(planes[1].x, planes[1].y, planes[1].z);
   length = glm::length(normal);
	planes[1] = planes[1] / length;

   // Bottom plane
	planes[2].x = comp[0][3] + comp[0][1];
	planes[2].y = comp[1][3] + comp[1][1];
	planes[2].z = comp[2][3] + comp[2][1];
	planes[2].w = comp[3][3] + comp[3][1];
   normal = vec3(planes[2].x, planes[2].y, planes[2].z);
   length = glm::length(normal);
	planes[2] = planes[2] / length;

   // Top plane
	planes[3].x = comp[0][3] - comp[0][1];
	planes[3].y = comp[1][3] - comp[1][1];
	planes[3].z = comp[2][3] - comp[2][1];
	planes[3].w = comp[3][3] - comp[3][1];
   normal = vec3(planes[3].x, planes[3].y, planes[3].z);
   length = glm::length(normal);
	planes[3] = planes[3] / length;

   // Near plane
	planes[4].x = comp[0][3] + comp[0][2];
	planes[4].y = comp[1][3] + comp[1][2];
	planes[4].z = comp[2][3] + comp[2][2];
	planes[4].w = comp[3][3] + comp[3][2];
   normal = vec3(planes[4].x, planes[4].y, planes[4].z);
   length = glm::length(normal);
	planes[4] = planes[4] / length;
	
   // Far plane
	planes[5].x = comp[0][3] - comp[0][2];
	planes[5].y = comp[1][3] - comp[1][2];
	planes[5].z = comp[2][3] - comp[2][2];
	planes[5].w = comp[3][3] - comp[3][2];
   normal = vec3(planes[5].x, planes[5].y, planes[5].z);
   length = glm::length(normal);
	planes[5] = planes[5] / length;
}

// See 'www.txutxi.com/?p=584' for detailed algorithm explanation
bool ViewFrustum::cull(std::shared_ptr<GameObject> obj) {
   /* Every object needs to have a bounding box in order to cull.
    * If an object doesn't have a bounding box, cull it so we don't create
    * unseen problems with culling. */
   BoundingBox* objBox = obj->getBoundingBox();
   if (objBox == NULL) {
      return false;
   }
   vec3 box[] = {objBox->min_, objBox->max_};

   vec4 plane;
   int px, py, pz;
   float dotProduct;
   for (int i = 0; i < planes.size(); ++i) {
      plane = planes[i];

      px = static_cast<int>(plane.x > 0.0f);
      py = static_cast<int>(plane.y > 0.0f);
      pz = static_cast<int>(plane.z > 0.0f);

      dotProduct = (plane.x * box[px].x) +
                   (plane.y * box[py].y) +
                   (plane.z * box[pz].z);

      if (dotProduct < -plane.w) {
         return true;
      }
   }

   return false;
}
