//
// Created by samuel on 02/02/18.
//

#include <glm/glm.hpp>

#ifndef MEDIEFIGHT_LEVEL_H
#define MEDIEFIGHT_LEVEL_H

#endif //MEDIEFIGHT_LEVEL_H

class Level {

public:
    void render(glm::mat4 projectionMatrix,
                glm::mat4 viewMatrix,
                glm::vec3 lightPosInEyeSpace,
                glm::vec3 cameraPosition);
    void update();
};