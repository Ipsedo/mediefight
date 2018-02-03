//
// Created by samuel on 19/01/18.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "modelvbo.h"
#include "utils/graphics/shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <utils/string_utils.h>
#include <utils/res.h>

ModelVBO::ModelVBO(std::string model_file_name) {
    init();
    bind();
    std::vector<std::string> dot_split = split(model_file_name, '.');
    std::string type = dot_split[dot_split.size() - 1];
    if (type == "obj")
        bindBuffer(parseObj(model_file_name));
    else if (type == "stl")
        bindBuffer(parseStl((model_file_name)));
    else
        perror("Unsuported file name !");

    lightCoef = 1;
    distanceCoef = 0;
    this->color = glm::vec4(std::rand(), std::rand(), std::rand(), 1.f);
    this->color /= RAND_MAX;
}

ModelVBO::ModelVBO(std::string model_file_name, glm::vec4 color) {
    init();
    bind();
    std::vector<std::string> dot_split = split(model_file_name, '.');
    std::string type = dot_split[dot_split.size() - 1];
    if (type == "obj")
        bindBuffer(parseObj(model_file_name));
    else if (type == "stl")
        bindBuffer(parseStl((model_file_name)));
    else
        perror("Unsuported file name !");

    lightCoef = 1;
    distanceCoef = 0;
    this->color = color;
}

void ModelVBO::init() {
    mProgram = glCreateProgram();
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, getResFolder() + "/shaders/diffuse_vs.glsl");
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, getResFolder() + "/shaders/diffuse_fs.glsl");
    glAttachShader(mProgram, vertexShader);
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);
}

void ModelVBO::bind() {
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgram, "u_MVMatrix");
    mPositionHandle = (GLuint) glGetAttribLocation(mProgram, "a_Position");
    mColorHandle = (GLuint) glGetUniformLocation(mProgram, "u_Color");
    mLightPosHandle = (GLuint) glGetUniformLocation(mProgram, "u_LightPos");
    mDistanceCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_distance_coef");
    mLightCoefHandle = (GLuint) glGetUniformLocation(mProgram, "u_light_coef");
    mNormalHandle = (GLuint) glGetAttribLocation(mProgram, "a_Normal");
}

void ModelVBO::bindBuffer(std::vector<float> packedData) {
    glGenBuffers(1, &packedDataBufferId);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glBufferData(GL_ARRAY_BUFFER, packedData.size() * BYTES_PER_FLOAT, &packedData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    packedData.clear();
}

std::vector<float> ModelVBO::parseObj(std::string objFileName) {
    nbVertex = 0;

    std::ifstream in(objFileName);

    if (!in) {
        std::cout << "Error during opening models file" << std::endl;
        exit(0);
    }
    std::string str;

    std::vector<float> vertex_list;
    std::vector<float> normal_list;
    std::vector<int> vertex_draw_order;
    std::vector<int> normal_draw_order;

    while (std::getline(in, str)) {
        //std::cout << str << std::endl;
        std::vector<std::string> splitted_line = split(str, ' ');
        if(!splitted_line.empty()) {
            if (splitted_line[0] == "vn") {
                normal_list.push_back(std::stof(splitted_line[1]));
                normal_list.push_back(std::stof(splitted_line[2]));
                normal_list.push_back(std::stof(splitted_line[3]));
            } else if (splitted_line[0] == "v") {
                vertex_list.push_back(std::stof(splitted_line[1]));
                vertex_list.push_back(std::stof(splitted_line[2]));
                vertex_list.push_back(std::stof(splitted_line[3]));
            } else if (splitted_line[0] == "f") {
                std::vector<std::string> face1 = split(splitted_line[1], '/');
                std::vector<std::string> face2 = split(splitted_line[2], '/');
                std::vector<std::string> face3 = split(splitted_line[3], '/');

                vertex_draw_order.push_back(std::stoi(face1[0]));
                vertex_draw_order.push_back(std::stoi(face2[0]));
                vertex_draw_order.push_back(std::stoi(face3[0]));

                normal_draw_order.push_back(std::stoi(face1[2]));
                normal_draw_order.push_back(std::stoi(face2[2]));
                normal_draw_order.push_back(std::stoi(face3[2]));

                face1.clear();
                face2.clear();
                face3.clear();
            }
        }
    }

    in.close();

    std::vector<float> packedData;

    for (int i = 0; i < vertex_draw_order.size(); i++) {
        packedData.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3]);
        packedData.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3 + 1]);
        packedData.push_back(vertex_list[(vertex_draw_order[i] - 1) * 3 + 2]);

        packedData.push_back(normal_list[(normal_draw_order[i] - 1) * 3]);
        packedData.push_back(normal_list[(normal_draw_order[i] - 1) * 3 + 1]);
        packedData.push_back(normal_list[(normal_draw_order[i] - 1) * 3 + 2]);
        nbVertex++;
    }

    vertex_list.clear();
    vertex_draw_order.clear();
    normal_list.clear();
    normal_draw_order.clear();

    return packedData;
}

float parseFloat(std::ifstream *file) {
    char tmp[4];
    (*file).read(tmp, 4);
    float res;
    memcpy(&res, tmp, sizeof(char) * 4);
    return res;
}

std::vector<float> ModelVBO::parseStl(std::string stlFileName) {
    std::vector<float> res;

    std::ifstream file(stlFileName);

    if (!file) {
        std::cout << "Error during opening models file" << std::endl;
        exit(0);
    }

    char header[80] = "";
    char N[4];

    file.read(header, 80);

    file.read(N, 4);
    unsigned int nb_triangles = 0;
    /*
     * Little Endian supposed !
     */
    for (int i = 3; i >= 0; i--) {
        nb_triangles = (nb_triangles << 8) + N[i];
    }

    for (unsigned int i = 0; i < nb_triangles; i++) {
        float n[3] = { parseFloat(&file), parseFloat(&file), parseFloat(&file) };
        for (int j = 0; j < 3; j++) {
            res.push_back(parseFloat(&file));
            res.push_back(parseFloat(&file));
            res.push_back(parseFloat(&file));
            res.push_back(n[0]);
            res.push_back(n[1]);
            res.push_back(n[2]);
        }

        char attribute[2];
        file.read(attribute, 2);
    }

    std::cout << res.size() << " " << res.max_size() << std::endl;

    nbVertex = nb_triangles * 3;
    return res;
}

void ModelVBO::draw(glm::mat4 mvp_matrix, glm::mat4 mv_matrix, glm::vec3 light_pos) {
    glUseProgram(mProgram);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, 0);

    glBindBuffer(GL_ARRAY_BUFFER, packedDataBufferId);
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, NORMAL_SIZE, GL_FLOAT, GL_FALSE,
            STRIDE, (char *)NULL + POSITION_SIZE * BYTES_PER_FLOAT);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, glm::value_ptr(mv_matrix));

    glUniform3fv(mLightPosHandle, 1, glm::value_ptr(light_pos));

    glUniform4fv(mColorHandle, 1, glm::value_ptr(color));

    glUniform1f(mDistanceCoefHandle, distanceCoef);

    glUniform1f(mLightCoefHandle, lightCoef);

    glDrawArrays(GL_TRIANGLES, 0, nbVertex);

    glDisableVertexAttribArray(mPositionHandle);
    glDisableVertexAttribArray(mColorHandle);
    glDisableVertexAttribArray(mNormalHandle);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
