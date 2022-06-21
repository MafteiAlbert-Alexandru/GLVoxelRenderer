#ifndef OBJECT_H
#define OBJECT_H
#include "shader.hpp"
class Object
{
public:
    virtual void setShader(Shader *shader)
    {
        m_shader = shader;
    }
    virtual void render() = 0;

protected:
    unsigned int VBO, VAO, EBO;
    Shader *m_shader;
};

#endif