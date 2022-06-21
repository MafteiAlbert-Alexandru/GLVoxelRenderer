#ifndef TEXTURE3D_H
#define TEXTURE3D_H
#include <glad/glad.h>
#include <cstdlib>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
class Texture3D
{
public:
    Texture3D(int width, int height, int depth) : m_width(width), m_height(height), m_depth(depth)
    {
        m_map = (glm::uvec4 *)calloc(sizeof(glm::uvec4), m_width * m_height * m_depth);
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // or GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
    ~Texture3D() { free(m_map); }
    glm::uvec4 *rawMap() { return m_map; }

    void setTexel(glm::uvec4 t, glm::uvec3 pos)
    {
        m_map[pos.x + pos.y * m_width + pos.z * m_height* m_width] = t;
    }
    glm::uvec4 &getTexel(glm::ivec3 pos)
    {
        return m_map[pos.x + pos.y * m_width + pos.z * m_height];
    }
    void update()
    {
        bind();
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32UI, m_width, m_height, m_depth, 0, GL_RGBA_INTEGER,  GL_UNSIGNED_INT, m_map);
        unbind();
    }
    void bind()
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture);
    }
    void unbind()
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
    unsigned int m_width, m_height, m_depth;

private:
    unsigned int m_texture;

    glm::uvec4 *m_map;
};

#endif