#include "ObjectManager.h"

#include <string>
#include "ShaderController.h"
#include "Cube.h"
#include "Quad.h"
#include "Light.h"

void ObjectManager::Add(GlObject* object)
{
    objectList.push_back(object);
}

void ObjectManager::RemoveObject(int index)
{
    std::cout << "Removing object at index = " << index << '\n';
    if (!objectList.empty())
    {
        objectList.erase(objectList.begin() + index);
    }
}

void ObjectManager::LoadObject(Geometry geom, std::string name, float pos[3], float rot[3], float scale[3])
{

    GlObject* object;
    switch (geom)
    {
        case CUBE: object = new Cube(); break;
        case QUAD: object = new Quad(); break;
        //case SPHERE: static_cast<Cube*>(object); break;
        case LIGHT: object = new Light(); break;
        case NONE: return;
        default: return;
    }

    // TODO get default texture
    Texture tempTex("Textures/wall.jpg");
    object->name = name;
    object->shader = shaderController.Get("generic");
    object->texture = tempTex;
    object->position = glm::make_vec3(pos);
    object->rotation = glm::make_vec3(rot);
    object->scale = glm::make_vec3(scale);

    objectList.push_back(object);
}

void ObjectManager::Draw()
{
    int i = 0; // For setting light UBO
    for (auto objectPtr: objectList)
    {
        if (objectPtr->isLight)
        {
            Light* light = static_cast<Light*>(objectPtr);

            glm::vec4 color = light->color;
            if (!light->isActive)
            {
                // If light is inactive, set color to 0
                // this is to overwrite old data stored
                // within the UBO
                color = glm::vec4(0.0f);
            }

            glBindBuffer(GL_UNIFORM_BUFFER, uboLights);

            // =======================================
            // Send in light info to light UBO
            // Position
            glBufferSubData(GL_UNIFORM_BUFFER,
                    3*sizeof(glm::vec4)*i,
                    sizeof(glm::vec4),
                    glm::value_ptr(light->position));

            // Color
            glBufferSubData(GL_UNIFORM_BUFFER,
                    3*sizeof(glm::vec4)*i + sizeof(glm::vec4),
                    sizeof(glm::vec4),
                    glm::value_ptr(color));

            // Attenuation factors
            glm::vec4 attenuation =
                glm::vec4(
                        static_cast<Light*>(light)->constant,
                        static_cast<Light*>(light)->linear,
                        static_cast<Light*>(light)->quadratic,
                        0.0f // padding
                        );

            //printf("(%f, %f, %f, %f)\n", attenuation.x, attenuation.y, attenuation.z, attenuation.w);
            glBufferSubData(GL_UNIFORM_BUFFER,
                    3*sizeof(glm::vec4)*i + 2*sizeof(glm::vec4),
                    sizeof(glm::vec4),
                    glm::value_ptr(attenuation));
            // =======================================

            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLights);

            light->shader->use();
            glUniform3fv(glGetUniformLocation(light->shader->ID, "lightColor"), 1, glm::value_ptr(static_cast<Light*>(light)->color));

            ++i;
        }
        objectPtr->Draw();
    }
}
