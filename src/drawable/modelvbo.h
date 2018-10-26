//
// Created by samuel on 19/01/18.
//

#ifndef MEDIEFIGHT_OBJVBO_H
#define MEDIEFIGHT_OBJVBO_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>

using namespace std;

/**
 * One color object with VBO
 * Use OBJ or STL file
 */
class ModelVBO {
private:
	// Data sizes
	const int POSITION_SIZE = 3;
	const int NORMAL_SIZE = 3;
	const int BYTES_PER_FLOAT = 4;
	const int STRIDE = (POSITION_SIZE + NORMAL_SIZE) * BYTES_PER_FLOAT;

	// GPU program handle
	GLuint mProgram;
	GLuint mPositionHandle;
	GLuint mNormalHandle;
	GLuint mColorHandle;
	GLuint mMVPMatrixHandle;
	GLuint mLightPosHandle;
	GLuint mMVMatrixHandle;
	GLuint mDistanceCoefHandle;
	GLuint mLightCoefHandle;

	// Number of vertex -> needed for draw
	int nbVertex;

	glm::vec4 color;

	float distanceCoef;
	float lightCoef;

	GLuint packedDataBufferId;

	void init();

	void bind();

	void bindBuffer(vector<float> packedData);

	vector<float> parseObj(string objFileName);

	vector<float> parseStl(string stlFileName);

public:
	ModelVBO(string model_file);

	ModelVBO(string model_file_name, glm::vec4 color);

	void draw(glm::mat4 mvp_Matrix, glm::mat4 mv_matrix, glm::vec3 light_pos);
};

#endif //MEDIEFIGHT_OBJVBO_H
