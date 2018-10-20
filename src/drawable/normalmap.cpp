//
// Created by samuel on 20/10/18.
//

#include "normalmap.h"

SquareMap::SquareMap(string textureFile, string normalsFile) {
	initTex(textureFile, normalsFile);
}

void SquareMap::initTex(string textureFile, string normalsFile) {
	libpng_image colorTex = readPNG(textureFile);
	imgRGB imgRGB1 = toColoredImage(colorTex);
	delete[] colorTex.data;
	delete[] colorTex.rowPtrs;

	libpng_image normalsTex = readPNG(normalsFile);
	imgRGB imgRGB2 = toColoredImage(colorTex);
	delete[] normalsTex.data;
	delete[] normalsTex.rowPtrs;

	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB1.width, imgRGB1.height, 0, GL_RGBA, GL_FLOAT, imgRGB1.colors);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB2.width, imgRGB2.height, 0, GL_RGBA, GL_FLOAT, imgRGB2.colors);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] imgRGB1.colors;
	delete[] imgRGB2.colors;
}

void SquareMap::initPrgm() {
	mProgram = glCreateProgram();
	// TODO create shaders
}

void SquareMap::draw(glm::mat4 mvpMatrix, glm::mat4 mvMatrix, glm::vec3 lightPos) {

}
