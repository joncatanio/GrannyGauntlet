#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <cfloat>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture.h"
#include "MaterialManager.h"

class Program;

class Shape
{
public:
	Shape();
	virtual ~Shape();
	void loadMesh(const std::string &meshName);
	void calculateNormals(int i);
	void init();
	void resize();
	void draw(const std::shared_ptr<Program> prog, std::shared_ptr<Material> defaultMtl);
	glm::vec3& getMin();
	glm::vec3& getMax();
	void findAndSetMinAndMax(glm::mat4 orientTransform = glm::mat4(1.0f));
	
private:

    void bindMtl(const std::shared_ptr<Program> prog, std::shared_ptr<Material> mtl);

	std::vector<std::vector<unsigned>> eleBuf = std::vector<std::vector<unsigned>>();
	std::vector<std::vector<float>> posBuf = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> norBuf = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> texBuf = std::vector<std::vector<float>>();
	std::vector<unsigned> eleBufID = std::vector<unsigned>();
	std::vector<unsigned> posBufID = std::vector<unsigned>();
	std::vector<unsigned> norBufID = std::vector<unsigned>();
	std::vector<unsigned> texBufID = std::vector<unsigned>();
	unsigned vaoID;

    std::vector<std::string> textureNames = std::vector<std::string>();
    std::map<std::string, Texture*> textures;
    std::vector<bool> materialPresent = std::vector<bool>();
    std::vector<std::shared_ptr<Material>> materials;

	glm::vec3 min;
	glm::vec3 max;
};

#endif
