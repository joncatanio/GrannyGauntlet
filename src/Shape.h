#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <cfloat>
#include <memory>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
	void draw(const std::shared_ptr<Program> prog) const;
	glm::vec3& getMin();
	glm::vec3& getMax();
	
private:
	std::vector<std::vector<unsigned>> eleBuf = std::vector<std::vector<unsigned>>();
	std::vector<std::vector<float>> posBuf = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> norBuf = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> texBuf = std::vector<std::vector<float>>();
	std::vector<unsigned> eleBufID = std::vector<unsigned>();
	std::vector<unsigned> posBufID = std::vector<unsigned>();
	std::vector<unsigned> norBufID = std::vector<unsigned>();
	std::vector<unsigned> texBufID = std::vector<unsigned>();
	std::vector<unsigned> vaoID = std::vector<unsigned>();

	glm::vec3 min;
	glm::vec3 max;

	void findAndSetMinAndMax();
};

#endif
