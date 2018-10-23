//
// Created by samuel on 20/10/18.
//

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "normalmap.h"
#include "../utils/graphics/shader.h"
#include "../utils/res.h"
#include "../utils/graphics/gl_utils.h"
#include "../utils/string_utils.h"

NormalMapModel::NormalMapModel(string objFile, string textureFile, string normalsFile) : textures(new GLuint[2]) {
	initPrgm();
	bind();
	initTex(move(textureFile), move(normalsFile));
	genBuffer(move(objFile));
}

void NormalMapModel::initTex(string textureFile, string normalsFile) {

	imgRGB imgRGB1 = loadImage(move(textureFile));

	imgRGB imgRGB2 = loadImage(move(normalsFile));

	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB1.width, imgRGB1.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgRGB1.colors);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB2.width, imgRGB2.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgRGB2.colors);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] imgRGB1.colors;
	delete[] imgRGB2.colors;
}

void NormalMapModel::initPrgm() {
	mProgram = glCreateProgram();
	GLuint vs = loadShader(GL_VERTEX_SHADER, getResFolder() + "/shaders/normal_map_vs.glsl");
	GLuint fs = loadShader(GL_FRAGMENT_SHADER, getResFolder() + "/shaders/normal_map_fs.glsl");
	glAttachShader(mProgram, vs);
	glAttachShader(mProgram, fs);
	glLinkProgram(mProgram);
}

void NormalMapModel::bind() {
	mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVPMatrix");
	mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVMatrix");

	mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
	mTextCoordHandle = (GLuint) glGetAttribLocation(mProgram, "a_TexCoord");
	mNormalHandle = (GLuint) glGetAttribLocation(mProgram, "a_Normal");

	mLightPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_LightPos");
	mCamPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_cam_pos");

	mDistanceCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_distance_coef");
	mLightCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_light_coef");

	mTexHandle = (GLuint) glGetUniformLocation(mProgram, "u_tex");
	mNormalMapHandle = (GLuint) glGetUniformLocation(mProgram, "u_normalMap");
}

void NormalMapModel::draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::vec3 lightPos, glm::vec3 camPos) {
	glUseProgram(mProgram);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(mPositionHandle);
	glVertexAttribPointer(mPositionHandle, POSITION_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, 0);

	glEnableVertexAttribArray(mNormalHandle);
	glVertexAttribPointer(mNormalHandle, NORMAL_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + POSITION_SIZE *  BYTES_PER_FLOAT);

	glEnableVertexAttribArray(mTextCoordHandle);
	glVertexAttribPointer(mTextCoordHandle, TEX_COORD_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + (POSITION_SIZE + NORMAL_SIZE) *  BYTES_PER_FLOAT);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

	glUniform3fv(mLightPosHandle, 1, glm::value_ptr(lightPos));

	glUniform3fv(mCamPosHandle, 1, glm::value_ptr(camPos));

	glUniform1f(mDistanceCoefHandle, 0.f);

	glUniform1f(mLightCoefHandle, 1.f);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(mTexHandle, 0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(mNormalMapHandle, 1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glDrawArrays(GL_TRIANGLES, 0, nbVertex);

	glDisableVertexAttribArray(mPositionHandle);
	glDisableVertexAttribArray(mTextCoordHandle);

	glBindTexture(GL_TEXTURE_2D, 0);
	checkGLError();
}

void NormalMapModel::genBuffer(string objFileName) {
	 vector<float> packedData = parseObj(move(objFileName));

	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, packedData.size() * BYTES_PER_FLOAT, packedData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

vector<float> NormalMapModel::parseObj(string objFileName) {
	nbVertex = 0;

	ifstream in(objFileName);

	if (!in) {
		cout << "Error during opening models file" << endl;
		exit(0);
	}
	string str;

	vector<float> vertex_list;
	vector<float> normal_list;
	vector<float> uv_list;
	vector<int> vertex_draw_order;
	vector<int> normal_draw_order;
	vector<int> uv_draw_order;

	while (getline(in, str)) {
		//cout << str << endl;
		vector<string> splitted_line = split(str, ' ');
		if(!splitted_line.empty()) {
			if (splitted_line[0] == "vn") {
				normal_list.push_back(stof(splitted_line[1]));
				normal_list.push_back(stof(splitted_line[2]));
				normal_list.push_back(stof(splitted_line[3]));
			} else if (splitted_line[0] == "vt") {
				uv_list.push_back(stof(splitted_line[1]));
				uv_list.push_back(stof(splitted_line[2]));
			} else if (splitted_line[0] == "v") {
				vertex_list.push_back(stof(splitted_line[1]));
				vertex_list.push_back(stof(splitted_line[2]));
				vertex_list.push_back(stof(splitted_line[3]));
			} else if (splitted_line[0] == "f") {
				vector<string> v1 = split(splitted_line[1], '/');
				vector<string> v2 = split(splitted_line[2], '/');
				vector<string> v3 = split(splitted_line[3], '/');

				vertex_draw_order.push_back(stoi(v1[0]));
				vertex_draw_order.push_back(stoi(v2[0]));
				vertex_draw_order.push_back(stoi(v3[0]));

				uv_draw_order.push_back(stoi(v1[1]));
				uv_draw_order.push_back(stoi(v2[1]));
				uv_draw_order.push_back(stoi(v3[1]));

				normal_draw_order.push_back(stoi(v1[2]));
				normal_draw_order.push_back(stoi(v2[2]));
				normal_draw_order.push_back(stoi(v3[2]));

				v1.clear();
				v2.clear();
				v3.clear();
			}
		}
		splitted_line.clear();
	}

	in.close();

	vector<float> packedData;
	for (int i = 0; i < vertex_draw_order.size(); i++) {
		packedData.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3]);
		packedData.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3 + 1]);
		packedData.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3 + 2]);

		packedData.push_back(normal_list[(normal_draw_order[i] - 1) * 3]);
		packedData.push_back(normal_list[(normal_draw_order[i] - 1) * 3 + 1]);
		packedData.push_back(normal_list[(normal_draw_order[i] - 1) * 3 + 2]);

		packedData.push_back(uv_list[(uv_draw_order[i] - 1) * 2]);
		packedData.push_back(uv_list[(uv_draw_order[i] - 1) * 2 + 1]);

		nbVertex++;
	}

	vertex_list.clear();
	vertex_draw_order.clear();
	normal_list.clear();
	normal_draw_order.clear();
	uv_list.clear();
	uv_draw_order.clear();

	return packedData;
}

NormalMapModel::~NormalMapModel() {
	glDeleteTextures(2, textures);
	// TODO desaloué images textures
}
