#ifndef CUBE_H
#define CUBE_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "GlObject.h"
#include "Shader.h"

class Cube : public GlObject
{
public:

    Cube(std::string _name, Shader _shader)
    {
        name.assign(_name);
        shader = _shader;
        InitRenderData();
    }

    void InitRenderData()
    {
        GLfloat vertices[] = {
            // Position           // UV         // Normals
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0, 0, -1,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0, 0, -1,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0, 0, -1,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0, 0, -1,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0, 0, -1,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0, 0, -1,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0, 0, 1,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0, 0, 1,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0, 0, 1,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0, 0, 1,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0, 0, 1,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0, 0, 1,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1, 0, 0,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1, 0, 0,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1, 0, 0,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1, 0, 0,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1, 0, 0,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1, 0, 0,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1, 0, 0,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1, 0, 0,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1, 0, 0,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1, 0, 0,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0, -1, 0,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0, -1, 0,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0, -1, 0,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0, -1, 0,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0, -1, 0,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0, -1, 0,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0, 1, 0,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0, 1, 0,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0, 1, 0,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0, 1, 0,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0, 1, 0,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0, 1, 0
        };

        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        // buffer objects allow us to send large batches of data at once to the GPU
        // so that we don't have to send data vertex by vertex
        GLuint VBO;
        glGenBuffers(1, &VBO);

        // copies vertex data in buffer's memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // parameter descriptions:
        // 1. Which vertex attrib we want to configure. Relates to the layout location
        // 2. Size of the vertex attribute so vec3 is 3 values.
        // 3. The type of data
        // 4. Do we want data to be normalized?
        // 5. Stride of data: the space between consecutive vertex attribs
        // 6. Offset of the attrib data. Needs to be casted to void*
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);   // position
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // texture
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // normals
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // unbinding VBO
        // this is allowed, since the call to glVertexAttribPointer registered VBO
        // as the vertex attribute's bound vertex buffer object so afterwards we
        // can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // unbinding VAO for later use
        glBindVertexArray(0);
    }

    void Draw(Texture &texture, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f))
    {
        this->shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.ID);
        glUniform1i(glGetUniformLocation(this->shader.ID, "tex1"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        // TODO
        //model = glm::rotate(model, rotate);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        glUniformMatrix4fv(glGetUniformLocation(this->shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Draw cube
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

};

#endif // CUBE_H
