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
	const int POSITION_SIZE = 3;
	const int TEX_COORD_SIZE = 2;
	const int BYTES_PER_FLOAT = 4;
	const int STRIDE = (POSITION_SIZE + TEX_COORD_SIZE) * BYTES_PER_FLOAT;

	GLuint mProgram;
	GLuint mPositionHandle;
	GLuint mTextCoordHandle;
	GLuint mMVPMatrixHandle;
	GLuint mLightPosHandle;
	GLuint mMVMatrixHandle;
	GLuint mDistanceCoefHandle;
	GLuint mLightCoefHandle;
	GLuint mTexHandle;
	GLuint mNormalMapHAndle;

	GLuint buffer;

	GLuint *textures;

	void initPrgm();
	void initTex(string textureFile, string normalsFile);
	void bind();
	void genBuffer();
public:
	SquareMap(string textureFile, string normalsFile);
	void draw(glm::mat4 mvpMatrix, glm::mat4 mvMatrix, glm::vec3 lightPos);
};


#endif //MEDIEFIGHT_NORMALMAP_H
