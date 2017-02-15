#include "Image.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(std::string path) {
    imageData = stbi_load( path.c_str() , &width, &height, &components, STBI_rgb_alpha );
}

Image::~Image() {
    stbi_image_free(imageData);
}

unsigned char* Image::getImageData() {
    return imageData;
}