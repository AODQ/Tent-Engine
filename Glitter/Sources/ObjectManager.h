#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <vector>

#include "GlObject.h"

class ObjectManager
{
public:

    ~ObjectManager()
    {
        // TODO crash
        //for (auto objectPtr : objectList)
        //{
        //    delete(objectPtr);
        //}
    }

    void Add(GlObject* object);
    void LoadObject(Geometry geom, std::string name, float pos[3], float rot[3], float scale[3]);
    void RemoveObject(int index);
    void Draw();

    std::vector<GlObject*> objectList;
    // TODO better way to do this with UBOs?
    // Maybe inside a resource manager?
    GLuint uboLights;
};

#endif // OBJECT_MANAGER_H
