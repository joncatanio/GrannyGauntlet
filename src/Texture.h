#ifndef TEXTURE_H
#define TEXTURE_H



#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture();
    virtual ~Texture();

    std::string name;

    // loads the texture under the specified path. channels = channels per pixel, 3 for RGB, 4 for RGBA
    void loadTexture(std::string path, std::string newName);
    void bind(int newUnit);
    void unbind();
    void setHandle(GLint h);
    GLint getHandle();


private:

    int width;
    int height;
    int components;

    GLuint tid;
    int unit;
    GLint handle;

};


#endif
