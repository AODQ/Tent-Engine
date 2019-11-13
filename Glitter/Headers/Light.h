#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public: 
    Light(glm::vec3 p, glm::vec3 c) : pos(p), color(c) {}
    Light(); 

    glm::vec3 pos;
    glm::vec3 color;
};

class DirLight : public Light
{
    DirLight(glm::vec3 c, glm::vec3 d) : direction(d) 
    {
        color = c;
    }

    glm::vec3 direction;
};

class PointLight : public Light
{
public:
    PointLight(glm::vec3 c, glm::vec3 p, float i) : pos(p), intensity(i) 
    {
        color = c;
    }

    glm::vec3 pos;
    float intensity;
};

#endif //LIGHT_H
