//
// Created by samuel on 20/10/18.
//

#ifndef MEDIEFIGHT_TEXTURES_H
#define MEDIEFIGHT_TEXTURES_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <SOIL/SOIL.h>

using namespace std;

struct imgRGB {
	int width;
	int height;
	unsigned char *colors;
};

imgRGB loadImage(string imageFile);

#endif //MEDIEFIGHT_TEXTURES_H
