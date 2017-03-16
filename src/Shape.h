#ifndef _SHAPE_H_
#define _SHAPE_H_

#define _USE_MATH_DEFINES

#include <cfloat>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "MatrixStack.h"

#include "Texture.h"
#include "MaterialManager.h"

class Program;
class GameObject;

class Shape
{
public:
	Shape();
	virtual ~Shape();
	void loadMesh(const std::string &meshName, std::vector<std::string> manualTextures);
	void calculateNormals(int i);
	void init();
	void resize();
	void draw(const std::shared_ptr<Program> prog, std::shared_ptr<Material> defaultMtl);
   std::shared_ptr<std::vector<glm::vec3>> calcFragmentDir(glm::vec3 direction);
   void fracture(const std::shared_ptr<Program> prog, std::shared_ptr<Material> defaultMtl,
      std::shared_ptr<MatrixStack> M, std::shared_ptr<GameObject> obj);
	glm::vec3& getMin();
	glm::vec3& getMax();
	void findAndSetMinAndMax(glm::mat4 orientTransform = glm::mat4(1.0f));
	
private:

    void bindMtl(const std::shared_ptr<Program> prog, std::shared_ptr<Material> mtl) const;

	std::vector<std::vector<unsigned>> eleBuf = std::vector<std::vector<unsigned>>();
	std::vector<std::vector<float>> posBuf = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> norBuf = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> texBuf = std::vector<std::vector<float>>();
	std::vector<unsigned> eleBufID = std::vector<unsigned>();
	std::vector<unsigned> posBufID = std::vector<unsigned>();
	std::vector<unsigned> norBufID = std::vector<unsigned>();
	std::vector<unsigned> texBufID = std::vector<unsigned>();
	unsigned vaoID;

   // For fracturing objects
   float randFloat(float a, float b);

	// Textures via mtl file
    std::vector<std::string> textureNames = std::vector<std::string>();
    std::map<std::string, Texture*> textures;
    std::vector<bool> materialPresent = std::vector<bool>();
    std::vector<std::shared_ptr<Material>> materials;

	// Textures "manually" provided via level JSON
	bool manualTexturePresent = false;
	//std::string manualTextureName;
    std::vector<std::string> manualTextureNames;

	glm::vec3 min;
	glm::vec3 max;
};

#endif
