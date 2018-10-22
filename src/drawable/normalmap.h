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

class NormalMapModel {
private:
	const int POSITION_SIZE = 3;
	const int TEX_COORD_SIZE = 2;
	const int NORMAL_SIZE = 3;
	const int TANGENT_SIZE = 3;
	const int BITANGENT_SIZE = 3;
	const int BYTES_PER_FLOAT = 4;
	const int STRIDE = (POSITION_SIZE + NORMAL_SIZE + TANGENT_SIZE + BITANGENT_SIZE + TEX_COORD_SIZE) * BYTES_PER_FLOAT;

	GLuint mProgram;
	GLuint mPositionHandle;
	GLuint mNormalHandle;
	GLuint mTangentHandle;
	GLuint mBiTangentHandle;
	GLuint mTextCoordHandle;
	GLuint mMVPMatrixHandle;
	GLuint mLightPosHandle;
	GLuint mMVMatrixHandle;
	GLuint mMMatrixHandle;
	GLuint mDistanceCoefHandle;
	GLuint mLightCoefHandle;
	GLuint mTexHandle;
	GLuint mNormalMapHandle;

	GLuint buffer;

	GLuint *textures;

	int nbVertex;

	void initPrgm();
	void initTex(string textureFile, string normalsFile);
	void bind();
	void genBuffer(string objFileName);
	vector<float> parseObj(string objFileName);
public:
	NormalMapModel(string objFile, string textureFile, string normalsFile);
	void draw(glm::mat4 mvpMatrix, glm::mat4 mvMatrix, glm::mat4 m_matrix,  glm::vec3 lightPos);
};


#endif //MEDIEFIGHT_NORMALMAP_H
