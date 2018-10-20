//
// Created by samuel on 20/10/18.
//

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "normalmap.h"
#include "../utils/graphics/shader.h"
#include "../utils/res.h"

SquareMap::SquareMap(string textureFile, string normalsFile) {
	glEnable(GL_TEXTURE_2D);
	initPrgm();
	initTex(textureFile, normalsFile);
	bind();
	genBuffer();
}

void SquareMap::initTex(string textureFile, string normalsFile) {
	libpng_image colorTex = readPNG(textureFile);
	imgRGB imgRGB1 = toColoredImage(colorTex);

	libpng_image normalsTex = readPNG(normalsFile);
	imgRGB imgRGB2 = toColoredImage(colorTex);

	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB1.width, imgRGB1.height, 0, GL_RGB, GL_FLOAT, imgRGB1.colors);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB2.width, imgRGB2.height, 0, GL_RGB, GL_FLOAT, imgRGB2.colors);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] imgRGB1.colors;
	delete[] imgRGB2.colors;

	delete[] normalsTex.data;
	delete[] normalsTex.rowPtrs;

	delete[] colorTex.data;
	delete[] colorTex.rowPtrs;
}

void SquareMap::initPrgm() {
	mProgram = glCreateProgram();
	GLuint vs = loadShader(GL_VERTEX_SHADER, getResFolder() + "/shaders/normal_map_vs.glsl");
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, getResFolder() + "/shaders/normal_map_fs.glsl");
	glAttachShader(mProgram, vs);
	glAttachShader(mProgram, fs);
	glLinkProgram(mProgram);
}

void SquareMap::bind() {
	mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVPMatrix");
	mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVMatrix");

	mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
	mTextCoordHandle = (GLuint) glGetAttribLocation(mProgram, "a_TexCoord");

	mLightPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_LightPos");

	mDistanceCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_distance_coef");
	mLightCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_light_coef");

	mTexHandle = (GLuint) glGetUniformLocation(mProgram, "u_tex");
	mNormalMapHAndle = (GLuint) glGetUniformLocation(mProgram, "u_normalMap");
}

void SquareMap::draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::vec3 lightPos) {
	glEnable(GL_TEXTURE_2D);

	glUseProgram(mProgram);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(mPositionHandle);
	glVertexAttribPointer(mPositionHandle, POSITION_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, 0);

	glEnableVertexAttribArray(mTextCoordHandle);
	glVertexAttribPointer(mTextCoordHandle, TEX_COORD_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + POSITION_SIZE * BYTES_PER_FLOAT);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

	glUniform3fv(mLightPosHandle, 1, glm::value_ptr(lightPos));

	glUniform1f(mDistanceCoefHandle, 0.f);

	glUniform1f(mLightCoefHandle, 1.f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(mTexHandle, textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(mNormalMapHAndle, textures[1]);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(mPositionHandle);
	glDisableVertexAttribArray(mTextCoordHandle);
}

void SquareMap::genBuffer() {
	float vertices[3 * 3 * 2 + 3 * 2 * 2] = {
			// Fst triangle
			-1.f, 1.f, 0.f, // fst vertex
			0.f, 1.f,       // fst text coord
			1.f, -1.f, 0.f,
			1.f, 0.f,
			-1.f, -1.f, 0.f,
			0.f, 0.f,

			// Snd triangle
			-1.f, 1.f, 0.f,
			0.f, 1.f,
			1.f, -1.f, 0.f,
			1.f, 0.f,
			1.f, 1.f, 0.f,
			1.f, 1.f

	};

	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (3 * 3 * 2 + 3 * 2 * 2) * BYTES_PER_FLOAT, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
