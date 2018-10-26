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
	const int BYTES_PER_FLOAT = 4;
	const int STRIDE = (POSITION_SIZE + NORMAL_SIZE + TEX_COORD_SIZE) * BYTES_PER_FLOAT;

	GLuint mProgram;
	GLuint mPositionHandle;
	GLuint mNormalHandle;
	GLuint mTextCoordHandle;
	GLuint mMVPMatrixHandle;
	GLuint mLightPosHandle;
	GLuint mCamPosHandle;
	GLuint mMVMatrixHandle;
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

	void draw(glm::mat4 mvpMatrix, glm::mat4 mvMatrix, glm::vec3 lightPos, glm::vec3 camPos);

	~NormalMapModel();
};

struct KeyFuncs {
	size_t operator()(const glm::vec3 &k) const {
		auto x = hash<float>()(k.x);
		auto y = hash<float>()(k.y > 0 ? k.y + 1.f : k.y - 1.f);
		auto z = hash<float>()(k.z > 0 ? k.z + 2.f : k.z - 2.f);
		return x ^ y ^ z;
	}

	bool operator()(const glm::vec3 &a, const glm::vec3 &b) const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

#endif //MEDIEFIGHT_NORMALMAP_H
