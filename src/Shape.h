#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <string>
#include <vector>
#include <memory>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

class Program;

class Shape
{
public:
	Shape();
	virtual ~Shape();
	void loadMesh(const std::string &meshName);
	void calculateNormals();
	void init();
	void resize();
	void draw(const std::shared_ptr<Program> prog) const;
	Eigen::Vector3f& getMin();
	Eigen::Vector3f& getMax();
	
private:
	std::vector<unsigned int> eleBuf;
	std::vector<float> posBuf;
	std::vector<float> norBuf;
	std::vector<float> texBuf;
	unsigned eleBufID;
	unsigned posBufID;
	unsigned norBufID;
	unsigned texBufID;
	unsigned vaoID;

	Eigen::Vector3f min;
	Eigen::Vector3f max;

	void findAndSetMinAndMax();
};

#endif
