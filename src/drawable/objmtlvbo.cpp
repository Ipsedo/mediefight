//
// Created by samuel on 02/02/18.
//

#include <GL/glew.h>
#include <utils/graphics/shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <utils/res.h>
#include <map>
#include <fstream>
#include <utils/string_utils.h>
#include <iostream>
#include "objmtlvbo.h"

void ObjMtlVBO::init() {
    mProgram = glCreateProgram();
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, getResFolder() + "/shaders/specular_vs.glsl");
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, getResFolder() + "/shaders/specular_fs.glsl");
    glAttachShader(mProgram, vertexShader);
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);
}

void ObjMtlVBO::bind() {
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVMatrix");
    mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
    mAmbColorHandle = (GLuint) glGetAttribLocation(mProgram, "a_material_ambient_Color");
    mDiffColorHandle = (GLuint) glGetAttribLocation(mProgram, "a_material_diffuse_Color");
    mSpecColorHandle = (GLuint) glGetAttribLocation(mProgram, "a_material_specular_Color");
    mLightPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_LightPos");
    mNormalHandle = (GLuint) glGetAttribLocation(mProgram, "a_Normal");
    mDistanceCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_distance_coef");
    mLightCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_light_coef");
    mCameraPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_CameraPosition");
    mSpecShininessHandle = (GLuint) glGetAttribLocation(mProgram, "a_material_shininess");
}

void ObjMtlVBO::bindBuffer(std::vector<float> packedData) {
    glGenBuffers(1, &packedDataBufferId);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glBufferData(GL_ARRAY_BUFFER, packedData.size() * BYTES_PER_FLOAT, &packedData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    packedData.clear();
}

std::vector<float> ObjMtlVBO::parseObj(std::string objFileName, std::string mtlFileName) {
    using namespace std;
    vector<float> res;
    map<string, glm::vec3> ambColor;
    map<string, glm::vec3> diffColor;
    map<string, glm::vec3> specColor;
    map<string, float> shin;

    ifstream mtlFile(mtlFileName);
    string str;

    std::string currMtl;
    while (std::getline(mtlFile, str)) {
        vector<string> splitted_line = split(str, ' ');
        if (!splitted_line.empty()) {
            if (splitted_line[0] == "newmtl") {
                currMtl = splitted_line[1];
            } else if (splitted_line[0] == "Ka") {
                pair<string, glm::vec3>
                        tmp(currMtl,
                            glm::vec3(strtof(splitted_line[1].c_str(), NULL),
                                      strtof(splitted_line[2].c_str(), NULL),
                                      strtof(splitted_line[3].c_str(), NULL))
                );
                ambColor.insert(tmp);
            } else if (splitted_line[0] == "Kd") {
                pair<string, glm::vec3>
                        tmp(currMtl,
                            glm::vec3(strtof(splitted_line[1].c_str(), NULL),
                                      strtof(splitted_line[2].c_str(), NULL),
                                      strtof(splitted_line[3].c_str(), NULL))
                );
                diffColor.insert(tmp);
            } else if (splitted_line[0] == "Ks") {
                pair<string, glm::vec3>
                        tmp(currMtl,
                            glm::vec3(strtof(splitted_line[1].c_str(), NULL),
                                      strtof(splitted_line[2].c_str(), NULL),
                                      strtof(splitted_line[3].c_str(), NULL))
                );
                specColor.insert(tmp);
            } else if (splitted_line[0] == "Ns") {
                pair<string, float> tmp(currMtl, strtof(splitted_line[1].c_str(), 0));
                shin.insert(tmp);
            }
        }
    }
    mtlFile.close();

    vector<float> currVertixsList;
    vector<float> currNormalsList;
    vector<int> currVertexDrawOrderList;
    vector<int> currNormalDrawOrderList;
    vector<vector<int>> allVertexDrawOrderList;
    vector<vector<int>> allNormalDrawOrderList;
    vector<string> mtlToUse;

    int idMtl = 0;

    ifstream objFile(objFileName);
    while (getline(objFile, str)) {
        vector<std::string> splitted_line = split(str, ' ');
        if (!splitted_line.empty()) {
            if (splitted_line[0] == "usemtl") {
                mtlToUse.push_back(splitted_line[1]);
                if (idMtl != 0) {
                    allVertexDrawOrderList.push_back(currVertexDrawOrderList);
                    allNormalDrawOrderList.push_back(currNormalDrawOrderList);
                }
                currVertexDrawOrderList.clear();
                currNormalDrawOrderList.clear();
                idMtl++;
            } else if (splitted_line[0] == "vn") {
                currNormalsList.push_back(strtof(splitted_line[1].c_str(), NULL));
                currNormalsList.push_back(strtof(splitted_line[2].c_str(), NULL));
                currNormalsList.push_back(strtof(splitted_line[3].c_str(), NULL));
            } else if (splitted_line[0] == "v") {
                currVertixsList.push_back(strtof(splitted_line[1].c_str(), NULL));
                currVertixsList.push_back(strtof(splitted_line[2].c_str(), NULL));
                currVertixsList.push_back(strtof(splitted_line[3].c_str(), NULL));
            } else if (splitted_line[0] == "f") {
                currVertexDrawOrderList.push_back(atoi(split(splitted_line[1], '/')[0].c_str()));
                currVertexDrawOrderList.push_back(atoi(split(splitted_line[2], '/')[0].c_str()));
                currVertexDrawOrderList.push_back(atoi(split(splitted_line[3], '/')[0].c_str()));

                currNormalDrawOrderList.push_back(atoi(split(splitted_line[1], '/')[2].c_str()));
                currNormalDrawOrderList.push_back(atoi(split(splitted_line[2], '/')[2].c_str()));
                currNormalDrawOrderList.push_back(atoi(split(splitted_line[3], '/')[2].c_str()));
            }
        }
    }
    allVertexDrawOrderList.push_back(currVertexDrawOrderList);
    allNormalDrawOrderList.push_back(currNormalDrawOrderList);
    objFile.close();

    nbVertex = 0;
    for (int i = 0; i < allVertexDrawOrderList.size(); i++) {
        glm::vec3 amb = glm::vec3(0.f);
        glm::vec3 diff = glm::vec3(0.f);
        glm::vec3 spec = glm::vec3(0.f);
        if (randomColor) {
            amb = glm::vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);
            diff = glm::vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);
            spec = glm::vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);
        } else {
            amb = ambColor.find(mtlToUse[i])->second;
            diff = diffColor.find(mtlToUse[i])->second;
            spec = specColor.find(mtlToUse[i])->second;
        }

        for (int j = 0; j < allVertexDrawOrderList[i].size(); j++) {
            res.push_back(currVertixsList[(allVertexDrawOrderList[i][j] - 1) * 3]);
            res.push_back(currVertixsList[(allVertexDrawOrderList[i][j] - 1) * 3 + 1]);
            res.push_back(currVertixsList[(allVertexDrawOrderList[i][j] - 1) * 3 + 2]);

            res.push_back(currNormalsList[(allNormalDrawOrderList[i][j] - 1) * 3]);
            res.push_back(currNormalsList[(allNormalDrawOrderList[i][j] - 1) * 3 + 1]);
            res.push_back(currNormalsList[(allNormalDrawOrderList[i][j] - 1) * 3 + 2]);

            res.push_back(amb[0]);
            res.push_back(amb[1]);
            res.push_back(amb[2]);
            res.push_back(1.f);

            res.push_back(diff[0]);
            res.push_back(diff[1]);
            res.push_back(diff[2]);
            res.push_back(1.f);

            res.push_back(spec[0]);
            res.push_back(spec[1]);
            res.push_back(spec[2]);
            res.push_back(1.f);

            res.push_back(shin.find(mtlToUse[i])->second);

            nbVertex++;
        }
    }

    return res;
}

ObjMtlVBO::ObjMtlVBO(std::string objFileName, std::string mtlFileName, bool randomColor) {
    this->randomColor = randomColor;
    init();
    bind();
    bindBuffer(parseObj(objFileName, mtlFileName));
    lightCoef = 1;
    distanceCoef = 0;
}

void ObjMtlVBO::draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::vec3 ligh_pos_in_eye_space, glm::vec3 camera_pos) {
    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          STRIDE, 0);

    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, NORMAL_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          STRIDE, (char *) NULL + POSITION_DATA_SIZE * BYTES_PER_FLOAT);

    glEnableVertexAttribArray(mAmbColorHandle);
    glVertexAttribPointer(mAmbColorHandle, COLOR_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          STRIDE, (char *) NULL + (POSITION_DATA_SIZE + NORMAL_DATA_SIZE) * BYTES_PER_FLOAT);

    glEnableVertexAttribArray(mDiffColorHandle);
    glVertexAttribPointer(mDiffColorHandle, COLOR_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          STRIDE,
                          (char *) NULL + (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE) * BYTES_PER_FLOAT);

    glEnableVertexAttribArray(mSpecColorHandle);
    glVertexAttribPointer(mSpecColorHandle, COLOR_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          STRIDE, (char *) NULL +
                                  (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE * 2) * BYTES_PER_FLOAT);

    glEnableVertexAttribArray(mSpecShininessHandle);
    glVertexAttribPointer(mSpecShininessHandle, SHININESS_DATA_SIZE, GL_FLOAT, GL_FALSE,
                          STRIDE, (char *) NULL +
                                  (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE * 3) * BYTES_PER_FLOAT);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, glm::value_ptr(mv_matrix));

    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform3fv(mLightPosHandle, 1, glm::value_ptr(ligh_pos_in_eye_space));

    glUniform3fv(mCameraPosHandle, 1, glm::value_ptr(camera_pos));

    glUniform1f(mDistanceCoefHandle, distanceCoef);

    glUniform1f(mLightCoefHandle, lightCoef);

    glDrawArrays(GL_TRIANGLES, 0, nbVertex);

    glDisableVertexAttribArray(mPositionHandle);
    glDisableVertexAttribArray(mNormalHandle);
    glDisableVertexAttribArray(mAmbColorHandle);
    glDisableVertexAttribArray(mDiffColorHandle);
    glDisableVertexAttribArray(mSpecColorHandle);
    glDisableVertexAttribArray(mSpecShininessHandle);
}
