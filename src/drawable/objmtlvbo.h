//
// Created by samuel on 02/02/18.
//

#ifndef MEDIEFIGHT_OBJMTLVBO_H
#define MEDIEFIGHT_OBJMTLVBO_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class ObjMtlVBO {

private:
	/**
	 * Constants
	 */
	const int POSITION_DATA_SIZE = 3;
	const int NORMAL_DATA_SIZE = 3;
	const int COLOR_DATA_SIZE = 4;
	const int SHININESS_DATA_SIZE = 1;
	const int BYTES_PER_FLOAT = 4;
	const int STRIDE =
			(POSITION_DATA_SIZE
			 + NORMAL_DATA_SIZE
			 + COLOR_DATA_SIZE * 3
			 + SHININESS_DATA_SIZE) * BYTES_PER_FLOAT;

	/**
	 * GL program handle
	 */
	GLuint mProgram;
	GLuint mPositionHandle;
	GLuint mNormalHandle;
	GLuint mAmbColorHandle;
	GLuint mDiffColorHandle;
	GLuint mSpecColorHandle;
	GLuint mSpecShininessHandle;
	GLuint mCameraPosHandle;
	GLuint mMVPMatrixHandle;
	GLuint mLightPosHandle;
	GLuint mMVMatrixHandle;
	GLuint mDistanceCoefHandle;
	GLuint mLightCoefHandle;

	/**
	 * Model infos
	 */
	float lightCoef;
	float distanceCoef;
	int nbVertex;

	/**
	 * VBO id
	 */
	GLuint packedDataBufferId;

	bool randomColor;

	void init();

	void bind();

	void bindBuffer(std::vector<float> packedData);

	std::vector<float> parseObj(std::string objFileName, std::string mtlFileName);

public:
	ObjMtlVBO(std::string objFileName, std::string mtlFileName, bool randomColor);

	void draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::vec3 ligh_pos_in_eye_space, glm::vec3 camera_pos);
};


#endif //MEDIEFIGHT_OBJMTLVBO_H
