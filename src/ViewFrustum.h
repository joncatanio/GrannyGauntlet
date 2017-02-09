#ifndef VIEW_FRUSTUM_H
#define VIEW_FRUSTUM_H

#include "BoundingBox.h"
#include <array>

class ViewFrustum {
public:
   ViewFrustum();

   ~ViewFrustum();

   void extractPlanes(glm::mat4 P, glm::mat4 V);

   bool cull(BoundingBox* box);

private:
   std::array<glm::vec4, 6> planes;
};

#endif
