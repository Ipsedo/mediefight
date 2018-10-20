//
// Created by samuel on 20/10/18.
//

#ifndef MEDIEFIGHT_TEXTURES_H
#define MEDIEFIGHT_TEXTURES_H

#include <png.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;

struct imgRGB {
	int width;
	int height;
	float *colors;
};

struct libpng_image {
	png_uint_32 width;
	png_uint_32 height;
	png_uint_32 bitdepth;
	png_uint_32 channels;
	char *data;
	png_bytep *rowPtrs;
};

libpng_image readPNG(std::string pngName);

imgRGB toColoredImage(libpng_image image);

#endif //MEDIEFIGHT_TEXTURES_H
