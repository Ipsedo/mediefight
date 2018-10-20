//
// Created by samuel on 20/10/18.
//

#ifndef MEDIEFIGHT_NORMALMAP_H
#define MEDIEFIGHT_NORMALMAP_H

#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>
#include "../utils/graphics/textures.h"

using namespace std;

class SquareMap {
private:
	GLuint mProgram;
	GLuint textures[2];
	void initPrgm();
	void initTex(string textureFile, string normalsFile);
public:
	SquareMap(string textureFile, string normalsFile);
	void draw(glm::mat4 mvpMatrix, glm::mat4 mvMatrix, glm::vec3 lightPos);
};


#endif //MEDIEFIGHT_NORMALMAP_H
