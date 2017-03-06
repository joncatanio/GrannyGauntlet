#include "Image.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Image::Image(std::string path) {
    imageData = stbi_load( path.c_str() , &width, &height, &components, STBI_rgb);

    if(imageData == nullptr) {
        std::cout << "failed to load " << path << std::endl;
    }
}

Image::~Image() {
    stbi_image_free(imageData);
}

unsigned char* Image::getImageData() {
    return imageData;
}