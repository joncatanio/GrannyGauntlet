#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image
{
public:
    Image(std::string path);
    virtual ~Image();

    int width;
    int height;
    int components;

    unsigned char* getImageData();

private:

    unsigned char* imageData;

};

#endif
