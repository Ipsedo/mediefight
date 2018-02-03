//
// Created by samuel on 02/02/18.
//

#include <GL/glew.h>
#include <utils/graphics/shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <utils/res.h>
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
    //TODO
    return std::vector<float>();
}

ObjMtlVBO::ObjMtlVBO(std::string objFileName, std::string mtlFileName, bool randomColor) {
    init();
    bind();
    //TODO
}

void ObjMtlVBO::draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::mat4 ligh_pos_in_eye_space, glm::mat4 camera_pos) {
    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_DATA_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, NORMAL_DATA_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char*) NULL + POSITION_DATA_SIZE * BYTES_PER_FLOAT);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mAmbColorHandle);
    glVertexAttribPointer(mAmbColorHandle, COLOR_DATA_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char*) NULL + (POSITION_DATA_SIZE + NORMAL_DATA_SIZE) * BYTES_PER_FLOAT);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mDiffColorHandle);
    glVertexAttribPointer(mDiffColorHandle, COLOR_DATA_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char*) NULL + (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE) * BYTES_PER_FLOAT);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mSpecColorHandle);
    glVertexAttribPointer(mSpecColorHandle, COLOR_DATA_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char*) NULL + (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE * 2) * BYTES_PER_FLOAT);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mSpecShininessHandle);
    glVertexAttribPointer(mSpecShininessHandle, SHININESS_DATA_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char*) NULL + (POSITION_DATA_SIZE + NORMAL_DATA_SIZE + COLOR_DATA_SIZE * 3) * BYTES_PER_FLOAT);

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
