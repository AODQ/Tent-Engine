#include <iostream>
#include <cstdio>
#include <fstream>

#include "SceneLoader.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"

#include "GlObject.h"
#include "Cube.h"
#include "Quad.h"
#include "Light.h"
#include "ObjectManager.h"
#include "ShaderController.h"

using namespace rapidjson;

extern ShaderController shaderController;

bool IsValidField(std::string field)
{
    return true;
}

void LoadSceneObjects(Value& sceneArray)
{
    std::cout << "Loading Objects\n";
}

Geometry ConvertStringToType(const char* type)
{
    if (strcmp(type, "CUBE") == 0)
    {
        return CUBE;
    }
    else if (strcmp(type, "QUAD") == 0)
    {
        return QUAD;
    }
    else if (strcmp(type, "SPHERE") == 0)
    {
        return SPHERE;
    }
    else if (strcmp(type, "LIGHT") == 0)
    {
        return LIGHT;
    }
    else if (strcmp(type, "MODEL") == 0)
    {
        return MODEL;
    }

    return NONE;
}

void SceneLoader::LoadScene(ObjectManager& manager, const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == 0)
    {
        std::cout << "ERROR: Failed to load SCENE file: " << path << '\n';
    }

    std::cout << "Clearing scene\n";
    for (auto objectPtr : manager.objectList)
    {
        delete objectPtr;
    }
    manager.objectList.clear();

    std::cout << "Loading SCENE file: " << path << '\n';
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document document;
    document.ParseStream(is);

    assert(document.IsObject());

    fclose(fp);

    const Value& sceneObjects = document["SceneObjects"];
    assert(sceneObjects.IsArray());
    // TODO Lights broken
    for (Value::ConstValueIterator itr = sceneObjects.Begin(); itr != sceneObjects.End(); ++itr)
    {
        GlObject* object;
        Geometry type = ConvertStringToType(itr->FindMember("type")->value.GetString());
        switch (type)
        {
            case CUBE: object = new Cube(); break;
            case QUAD: object = new Quad(); break;
                        //case SPHERE: object = new Sphere(); break;
            case LIGHT:
                       object = new Light(); break;
                       static_cast<Light*>(object)->color = glm::vec4(1.0);
            case NONE:
                        std::cout << "ERROR: Loading object with no type specified\n";
                        break;
            default:
                        std::cout << "ERROR: Loading object with no type specified\n";
                        break;
        }

        object->name = std::string(itr->FindMember("tag")->value.GetString());

        {
            const Value& a = itr->FindMember("position")->value;
            assert(a.IsArray());
            object->position = glm::vec3(a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble());
        }

        {
            const Value& a = itr->FindMember("rotation")->value;
            assert(a.IsArray());
            object->rotation = glm::vec3(a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble());
        }

        {
            const Value& a = itr->FindMember("scale")->value;
            assert(a.IsArray());
            object->scale = glm::vec3(a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble());
        }

        Texture texture(itr->FindMember("texture")->value.GetString());
        object->texture = texture;

        Shader* shader = shaderController.Get(std::string(itr->FindMember("shader")->value.GetString()));
        object->shader = shader;

        object->isActive = itr->FindMember("isActive")->value.GetBool();

        object->isLight = itr->FindMember("isLight")->value.GetBool();

        if (!object->isLight) // dont include lights for now
            manager.Add(object);
    }
}

void SceneLoader::SaveScene(ObjectManager& manager, const char* path)
{
    Document doc;
    doc.SetObject();
    Value myArray(kArrayType);
    Document::AllocatorType& allocator = doc.GetAllocator();

    for (const auto object : manager.objectList)
    {
        Value objValue;
        objValue.SetObject();
        // Write all fields of GlObject

        objValue.AddMember("type", "CUBE", allocator);

        Value tag(object->name.c_str(), allocator);

        objValue.AddMember("tag", tag, allocator);


        Value position(kArrayType);
        for (int i = 0; i < 3; ++i)
        {
            position.PushBack(Value().SetDouble(object->position[i]), allocator);
        }
        objValue.AddMember("position", position, allocator);


        Value rotation(kArrayType);
        for (int i = 0; i < 3; ++i)
        {
            rotation.PushBack(Value().SetDouble(object->rotation[i]), allocator);
        }
        objValue.AddMember("rotation", rotation, allocator);


        Value scale(kArrayType);
        for (int i = 0; i < 3; ++i)
        {
            scale.PushBack(Value().SetDouble(object->scale[i]), allocator);
        }
        objValue.AddMember("scale", scale, allocator);


        Value texture(object->texture.GetName().c_str(), allocator);
        objValue.AddMember("texture", texture, allocator);

        objValue.AddMember("shader", "generic", allocator);

        objValue.AddMember("isActive", object->isActive, allocator);

        objValue.AddMember("isLight", object->isLight, allocator);

        myArray.PushBack(objValue, allocator);
    }

    doc.AddMember("SceneObjects", myArray, allocator);

    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::ofstream out(path);
    out << buffer.GetString();
    out.close();
}

void SceneLoader::DeleteScene()
{

}
