#include "Image.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>

Image::Image(std::string path) {

    // split string to check file type fo texture
    std::vector<std::string> tokens;
    std::stringstream ss(path);
    std::string tok;
    while(getline(ss, tok, '.')) {
        tokens.push_back(tok);
    }
    std::string imgType = tokens.back();
    std::transform(imgType.begin(), imgType.end(), imgType.begin(), ::toupper);

    stbi_set_flip_vertically_on_load(true);
    // load with or without alpha channel
    //TODO(nurgan) find out what formats besides PNG use an alpha channel
    if(imgType == "PNG"){
        imageData = stbi_load( path.c_str() , &width, &height, &components, STBI_rgb_alpha);
    } else {
        imageData = stbi_load( path.c_str() , &width, &height, &components, STBI_rgb);
    }

}

Image::~Image() {
    stbi_image_free(imageData);
}

unsigned char* Image::getImageData() {
    return imageData;
}