
#include "MatrixStack.h"
#include <glm/gtc/matrix_transform.hpp>

MatrixStack::MatrixStack() {

   mstack = std::make_shared< std::stack<glm::mat4> >();
   mstack->push(glm::mat4(1.0));
}

MatrixStack::~MatrixStack() { }

void MatrixStack::pushMatrix()
{
	const glm::mat4 &top = mstack->top();
   mstack->push(top);
   assert(mstack->size() < 100);
}

void MatrixStack::popMatrix()
{
   assert(!mstack->empty());
   mstack->pop();
   // There should always be one matrix left.
   assert(!mstack->empty());
}

void MatrixStack::loadIdentity()
{
   glm::mat4 &top = mstack->top();
   top = glm::mat4(1.0);
}

void MatrixStack::translate(const glm::vec3 &offset)
{
   glm::mat4 &top = mstack->top();
	glm::mat4 t = glm::translate(glm::mat4(1.0), offset);
	top *= t;
}

void MatrixStack::scale(const glm::vec3 &scaleV)
{
   glm::mat4 &top = mstack->top();
	glm::mat4 s = glm::scale(glm::mat4(1.0), scaleV);
	top *= s;
}

void MatrixStack::scale(float size)
{
   glm::mat4 &top = mstack->top();
	glm::mat4 s = glm::scale(glm::mat4(1.0), glm::vec3(size));
	top *= s;
}

void MatrixStack::rotate(float angle, const glm::vec3 &axis)
{
   glm::mat4 &top = mstack->top();
	glm::mat4 r = glm::rotate(glm::mat4(1.0), angle, axis);
	top *= r;
}

void MatrixStack::rotateMat4(glm::mat4 &rotMat) {
   glm::mat4 &top = mstack->top();
   top *= rotMat;
}

void MatrixStack::multMatrix(const glm::mat4 &matrix)
{
   glm::mat4 &top = mstack->top();
	top *= matrix;
}

void MatrixStack::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
   assert(left != right);
   assert(bottom != top);
   assert(zFar != zNear);
   glm::mat4 &ctm = mstack->top();
   ctm *= glm::ortho(left, right, bottom, top, zNear, zFar);
}

void MatrixStack::frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
   glm::mat4 &ctm = mstack->top();
	ctm *= glm::frustum(left, right, bottom, top, zNear, zFar);
}

void MatrixStack::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up)
{
   glm::mat4 &top = mstack->top();
	top *= glm::lookAt(eye, target, up);
}

void MatrixStack::perspective(float fovy, float aspect, float zNear, float zFar)
{
   glm::mat4 &top = mstack->top();
	top *= glm::perspective(fovy, aspect, zNear, zFar);
}

const glm::mat4 &MatrixStack::topMatrix()
{
	return mstack->top();
}

void MatrixStack::print(const glm::mat4 &mat, const char *name) const
{
   if(name) {
      printf("%s = [\n", name);
   }
   for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
         printf("%- 5.2f ", mat[i][j]);
      }
      printf("\n");
   }
   if(name) {
      printf("];");
   }
   printf("\n");
}

void MatrixStack::print(const char *name) const
{
   print(mstack->top(), name);
}

