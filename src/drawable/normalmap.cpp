//
// Created by samuel on 20/10/18.
//

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include "normalmap.h"
#include "../utils/graphics/shader.h"
#include "../utils/res.h"
#include "../utils/graphics/gl_utils.h"
#include "../utils/string_utils.h"

NormalMapModel::NormalMapModel(string objFile, string textureFile, string normalsFile) : textures(new GLuint[2]) {
	initPrgm();
	initTex(move(textureFile), move(normalsFile));
	bind();
	genBuffer(objFile);
}

void NormalMapModel::initTex(string textureFile, string normalsFile) {

	imgRGB imgRGB1 = loadImage(move(textureFile));

	imgRGB imgRGB2 = loadImage(move(normalsFile));

	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgRGB1.width, imgRGB1.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgRGB1.colors);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
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
	mMMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MMatrix");

	mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
	mTextCoordHandle = (GLuint) glGetAttribLocation(mProgram, "a_TexCoord");
	mNormalHandle = (GLuint) glGetAttribLocation(mProgram, "a_Normal");
	mTangentHandle = (GLuint) glGetAttribLocation(mProgram, "a_Tangent");
	mBiTangentHandle = (GLuint) glGetAttribLocation(mProgram, "a_BiTangent");

	mLightPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_LightPos");

	mDistanceCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_distance_coef");
	mLightCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_light_coef");

	mTexHandle = (GLuint) glGetUniformLocation(mProgram, "u_tex");
	mNormalMapHandle = (GLuint) glGetUniformLocation(mProgram, "u_normalMap");
}

void NormalMapModel::draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::mat4 m_matrix, glm::vec3 lightPos) {
	glUseProgram(mProgram);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(mPositionHandle);
	glVertexAttribPointer(mPositionHandle, POSITION_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, 0);

	glEnableVertexAttribArray(mNormalHandle);
	glVertexAttribPointer(mNormalHandle, NORMAL_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + POSITION_SIZE *  BYTES_PER_FLOAT);

	glEnableVertexAttribArray(mTangentHandle);
	glVertexAttribPointer(mTangentHandle, TANGENT_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + (POSITION_SIZE + NORMAL_SIZE) *  BYTES_PER_FLOAT);

	glEnableVertexAttribArray(mBiTangentHandle);
	glVertexAttribPointer(mBiTangentHandle, BITANGENT_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + (POSITION_SIZE + NORMAL_SIZE + TANGENT_SIZE) *  BYTES_PER_FLOAT);

	glEnableVertexAttribArray(mTextCoordHandle);
	glVertexAttribPointer(mTextCoordHandle, TEX_COORD_SIZE, GL_FLOAT, GL_FALSE,
						  STRIDE, (char *)NULL + (POSITION_SIZE + NORMAL_SIZE + TANGENT_SIZE + BITANGENT_SIZE) *  BYTES_PER_FLOAT);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUniformMatrix4fv(mMMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_matrix));

	glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, glm::value_ptr(mv_matrix));

	glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

	glUniform3fv(mLightPosHandle, 1, glm::value_ptr(lightPos));

	glUniform1f(mDistanceCoefHandle, 0.f);

	glUniform1f(mLightCoefHandle, 1.f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(mTexHandle, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(mNormalMapHandle, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDrawArrays(GL_TRIANGLES, 0, nbVertex);

	glDisableVertexAttribArray(mPositionHandle);
	glDisableVertexAttribArray(mTextCoordHandle);
	//checkGLError();
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

	vector<glm::vec3> tangents;
	vector<glm::vec3> bitangents;
	for (int i = 0; i < vertex_draw_order.size(); i+=3) {
		glm::vec3 v0;
		v0.x = vertex_list[(vertex_draw_order[i] - 1) * 3];
		v0.y = vertex_list[(vertex_draw_order[i] - 1) * 3 + 1];
		v0.z = vertex_list[(vertex_draw_order[i] - 1) * 3 + 2];

		glm::vec3 v1;
		v1.x = vertex_list[(vertex_draw_order[i + 1] - 1) * 3];
		v1.y = vertex_list[(vertex_draw_order[i + 1] - 1) * 3 + 1];
		v1.z = vertex_list[(vertex_draw_order[i + 1] - 1) * 3 + 2];

		glm::vec3 v2;
		v2.x = vertex_list[(vertex_draw_order[i + 2] - 1) * 3];
		v2.y = vertex_list[(vertex_draw_order[i + 2] - 1) * 3 + 1];
		v2.z = vertex_list[(vertex_draw_order[i + 2] - 1) * 3 + 2];

		// UVs
		glm::vec2 uv0;
		uv0.x = uv_list[(uv_draw_order[i] - 1) * 3];
		uv0.y = uv_list[(uv_draw_order[i] - 1) * 3 + 1];

		glm::vec2 uv1;
		uv1.x = uv_list[(uv_draw_order[i + 1] - 1) * 3];
		uv1.y = uv_list[(uv_draw_order[i + 1] - 1) * 3 + 1];

		glm::vec2 uv2;
		uv2.x = uv_list[(uv_draw_order[i + 2] - 1) * 3];
		uv2.y = uv_list[(uv_draw_order[i + 2] - 1) * 3 + 1];

		// http://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-13-normal-mapping/
		glm::vec3 deltaPos1 = v1-v0;
		glm::vec3 deltaPos2 = v2-v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1-uv0;
		glm::vec2 deltaUV2 = uv2-uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}


	vector<float> packedData;

	for (int i = 0; i < vertex_draw_order.size(); i++) {
		// Vertices
		glm::vec3 v0;
		v0.x = vertex_list[(vertex_draw_order[i] - 1) * 3];
		v0.y = vertex_list[(vertex_draw_order[i] - 1) * 3 + 1];
		v0.z = vertex_list[(vertex_draw_order[i] - 1) * 3 + 2];

		// Normal
		glm::vec3 n0;
		n0.x = normal_list[(normal_draw_order[i] - 1) * 3];
		n0.y = normal_list[(normal_draw_order[i] - 1) * 3 + 1];
		n0.z = normal_list[(normal_draw_order[i] - 1) * 3 + 2];

		// UVs
		glm::vec2 uv0;
		uv0.x = uv_list[(uv_draw_order[i] - 1) * 3];
		uv0.y = uv_list[(uv_draw_order[i] - 1) * 3 + 1];

		packedData.push_back(v0.x);
		packedData.push_back(v0.y);
		packedData.push_back(v0.z);

		packedData.push_back(n0.x);
		packedData.push_back(n0.y);
		packedData.push_back(n0.z);

		packedData.push_back(tangents[i].x);
		packedData.push_back(tangents[i].y);
		packedData.push_back(tangents[i].z);

		packedData.push_back(bitangents[i].x);
		packedData.push_back(bitangents[i].y);
		packedData.push_back(bitangents[i].z);

		packedData.push_back(uv0.x);
		packedData.push_back(uv0.y);

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
