#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GlObject
{
public:
    virtual void Draw(Texture &texture, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f)) = 0;

    virtual void InitRenderData() = 0;

    Shader shader;
    GLuint VAO;
};

#endif
