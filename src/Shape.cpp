#include "Shape.h"
#include <iostream>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

#include "GLSL.h"
#include "Program.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

Shape::Shape() :
	eleBufID(0),
	posBufID(0),
	norBufID(0),
	texBufID(0), 
	vaoID(0),
	min(Eigen::Vector3f(0,0,0)),
	max(Eigen::Vector3f(0, 0, 0))
{
}

Shape::~Shape()
{
}

void Shape::loadMesh(const string &meshName) {
	// Load geometry
	// Some obj files contain material information.
	// We'll ignore them for this assignment.
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> objMaterials;
	string errStr;
	bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		posBuf = shapes[0].mesh.positions;
		//texBuf = shapes[0].mesh.texcoords;
		eleBuf = shapes[0].mesh.indices;

		calculateNormals();

		findAndSetMinAndMax();
	}
}

/**
 * Produces accurate normals from the vertices of the provided shape mesh
 *
 * Taken and modified from provided "lighting slides" pdf on PolyLearn along
 * with tips given on the PolyLearn forms by Prof. Wood
 */
void Shape::calculateNormals() {
	float v1[3], v2[3], nor[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;
	float length;

	// Set all vertex normals to zero so we can add the adjacent face normals to each
	norBuf.clear();
	norBuf.resize(posBuf.size());

	for (size_t v = 0; v < norBuf.size(); ++v) {
		norBuf[0] = 0.0;
	}

	// Calculate the normal for each face; add to adjacent vertices
	for (size_t v = 0; v < eleBuf.size() / 3; ++v) {
		int idx1 = eleBuf[3*v+0];
		int	idx2 = eleBuf[3*v+1];
		int	idx3 = eleBuf[3*v+2];

		// Calculate two vectors from the three points
		v1[x] = posBuf[3*idx1+x] - posBuf[3*idx2+x];
		v1[y] = posBuf[3*idx1+y] - posBuf[3*idx2+y];
		v1[z] = posBuf[3*idx1+z] - posBuf[3*idx2+z];

		v2[x] = posBuf[3*idx2+x] - posBuf[3*idx3+x];
		v2[y] = posBuf[3*idx2+y] - posBuf[3*idx3+y];
		v2[z] = posBuf[3*idx2+z] - posBuf[3*idx3+z];

		// Take the cross product of the two vectors to get
		// the normal vector which will be stored in out
		nor[x] = v1[y]*v2[z] - v1[z]*v2[y];
		nor[y] = v1[z]*v2[x] - v1[x]*v2[z];
		nor[z] = v1[x]*v2[y] - v1[y]*v2[x];

		// Normalize the vector
		length = sqrt(nor[x] * nor[x] + nor[y] * nor[y] + nor[z] * nor[z]);
		
		nor[x] /= length;
		nor[y] /= length;
		nor[z] /= length;

		// Set the normal into the shape's normal buffer
		norBuf[3*idx1+x] += nor[x];
		norBuf[3*idx1+y] += nor[y];
		norBuf[3*idx1+z] += nor[z];
		
		norBuf[3*idx2+x] += nor[x];
		norBuf[3*idx2+y] += nor[y];
		norBuf[3*idx2+z] += nor[z];

		norBuf[3*idx3+x] += nor[x];
		norBuf[3*idx3+y] += nor[y];
		norBuf[3*idx3+z] += nor[z];
	}

	// Normalize each vector's normal, effectively giving us a weighted average based on
	// the area of each face adjacent to each vertex
	for (size_t v = 0; v < norBuf.size() / 3; ++v) {
		nor[x] = norBuf[(v*3)+x];
		nor[y] = norBuf[(v*3)+y];
		nor[z] = norBuf[(v*3)+z];

		length = sqrt(nor[x] * nor[x] + nor[y] * nor[y] + nor[z] * nor[z]);

		nor[x] /= length;
		nor[y] /= length;
		nor[z] /= length;

		norBuf[(v*3)+x] = nor[x];
		norBuf[(v*3)+y] = nor[y];
		norBuf[(v*3)+z] = nor[z];
	}
}

void Shape::resize() {
	float scaleX, scaleY, scaleZ;
	float shiftX, shiftY, shiftZ;
	float epsilon = 0.001;

	// From min and max compute necessary scale and shift for each dimension
	float maxExtent, xExtent, yExtent, zExtent;
	xExtent = max.x() - min.x();
	yExtent = max.y() - min.y();
	zExtent = max.z() - min.z();

	if (xExtent >= yExtent && xExtent >= zExtent) {
		maxExtent = xExtent;
	}
	if (yExtent >= xExtent && yExtent >= zExtent) {
		maxExtent = yExtent;
	}
	if (zExtent >= xExtent && zExtent >= yExtent) {
		maxExtent = zExtent;
	}

	scaleX = 2.0 / maxExtent;
	shiftX = min.x() + (xExtent / 2.0);
	scaleY = 2.0 / maxExtent;
	shiftY = min.y() + (yExtent / 2.0);
	scaleZ = 2.0 / maxExtent;
	shiftZ = min.z() + (zExtent / 2.0);

	// Go through all verticies shift and scale them
	for (size_t v = 0; v < posBuf.size() / 3; v++) {
		posBuf[3*v+0] = (posBuf[3*v+0] - shiftX) * scaleX;
		assert(posBuf[3*v+0] >= -1.0 - epsilon);
		assert(posBuf[3*v+0] <= 1.0 + epsilon);

		posBuf[3*v+1] = (posBuf[3*v+1] - shiftY) * scaleY;
		assert(posBuf[3*v+1] >= -1.0 - epsilon);
		assert(posBuf[3*v+1] <= 1.0 + epsilon);

		posBuf[3*v+2] = (posBuf[3*v+2] - shiftZ) * scaleZ;
		assert(posBuf[3*v+2] >= -1.0 - epsilon);
		assert(posBuf[3*v+2] <= 1.0 + epsilon);
	}

	// Shift and scale min and max values
	float minX = (min.x() - shiftX) * scaleX;
	float minY = (min.y() - shiftY) * scaleY;
	float minZ = (min.z() - shiftZ) * scaleZ;
	min = Eigen::Vector3f(minX, minY, minZ);

	float maxX = (max.x() - shiftX) * scaleX;
	float maxY = (max.y() - shiftY) * scaleY;
	float maxZ = (max.z() - shiftZ) * scaleZ;
	max = Eigen::Vector3f(maxX, maxY, maxZ);
}

void Shape::init() {
	// Initialize the vertex array object
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	// Send the position array to the GPU
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the normal array to the GPU
	if (norBuf.empty()) {
		norBufID = 0;
	} else {
		glGenBuffers(1, &norBufID);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	}
	
	// Send the texture array to the GPU
	if (texBuf.empty()) {
		texBufID = 0;
	} else {
		glGenBuffers(1, &texBufID);
		glBindBuffer(GL_ARRAY_BUFFER, texBufID);
		glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
	}
	
	// Send the element array to the GPU
	glGenBuffers(1, &eleBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	assert(glGetError() == GL_NO_ERROR);
}

void Shape::draw(const shared_ptr<Program> prog) const {
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

	glBindVertexArray(vaoID);
	// Bind position buffer
	h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	
	// Bind normal buffer
	h_nor = prog->getAttribute("vertNor");
	if (h_nor != -1 && norBufID != 0) {
		GLSL::enableVertexAttribArray(h_nor);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	}

	if (texBufID != 0) {	
		// Bind texcoords buffer
		h_tex = prog->getAttribute("vertTex");
		if (h_tex != -1 && texBufID != 0) {
			GLSL::enableVertexAttribArray(h_tex);
			glBindBuffer(GL_ARRAY_BUFFER, texBufID);
			glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
		}
	}
	
	// Bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	
	// Draw
	glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0);
	
	// Disable and unbind
	if (h_tex != -1) {
		GLSL::disableVertexAttribArray(h_tex);
	}
	if (h_nor != -1) {
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


Eigen::Vector3f& Shape::getMin() {
	return this->min;
}

Eigen::Vector3f& Shape::getMax() {
	return this->max;
}

void Shape::findAndSetMinAndMax() {
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = FLT_MIN;

	// Go through all vertices to determine min and max of each dimension
	for (size_t v = 0; v < posBuf.size() / 3; v++) {
		if (posBuf[3 * v + 0] < minX) minX = posBuf[3 * v + 0];
		if (posBuf[3 * v + 0] > maxX) maxX = posBuf[3 * v + 0];

		if (posBuf[3 * v + 1] < minY) minY = posBuf[3 * v + 1];
		if (posBuf[3 * v + 1] > maxY) maxY = posBuf[3 * v + 1];

		if (posBuf[3 * v + 2] < minZ) minZ = posBuf[3 * v + 2];
		if (posBuf[3 * v + 2] > maxZ) maxZ = posBuf[3 * v + 2];
	}

	min = Eigen::Vector3f(minX, minY, minZ);
	max = Eigen::Vector3f(maxX, maxY, maxZ);
}
