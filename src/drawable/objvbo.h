//
// Created by samuel on 19/01/18.
//

#include <glm.hpp>

#ifndef MEDIEFIGHT_OBJVBO_H
#define MEDIEFIGHT_OBJVBO_H

#endif //MEDIEFIGHT_OBJVBO_H

class ObjVBO {

private:
    // Data sizes
    const int POSITION_SIZE = 3;
    const int NORMAL_SIZE = 3;
    const int BYTES_PER_FLOAT = 4;
    const int STRIDE = (POSITION_SIZE + NORMAL_SIZE) * BYTES_PER_FLOAT;

    // GPU program handle
    int mProgram;
    int mPositionHandle;
    int mNormalHandle;
    int mColorHandle;
    int mMVPMatrixHandle;
    int mLightPosHandle;
    int mMVMatrixHandle;
    int mDistanceCoefHandle;
    int mLightCoefHandle;

    // Number of vertex -> needed for draw
    int nbVertex;

    void init();
    void bind();
    void bindBuffer();

public:
    void draw(glm::mat4 mvp_Matrix, glm::mat4 mv_matrix, glm::vec3 light_pos);
};
