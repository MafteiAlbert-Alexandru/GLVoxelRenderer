#ifndef OCTREE_HPP
#define OCTREE_HPP
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cassert>
#include <iostream>
class Octree
{
public:
    struct octree_node
    {
        octree_node *children[8] = {nullptr};
        glm::u8vec4 *data = nullptr;
    };

    Octree(const glm::ivec3 size) : m_size(size), m_nodecount(1), m_leafcount(0)
    {
        assert(("Octree size must be power of 2 on all dimensions.",
                size.x % 2 == 0 && size.y % 2 == 0 && size.z % 2 == 0));
        m_root = new octree_node;
        glGenBuffers(1, &TBO);
        glGenTextures(1, &TBO_texture);
    }
    void set(glm::u8vec4 data, glm::ivec3 coordinates)
    {
        assert(("Insert out of bound", 0 <= coordinates.x && coordinates.x <= m_size.x &&
                                           0 <= coordinates.y && coordinates.y <= m_size.y &&
                                           0 <= coordinates.z && coordinates.z <= m_size.z));

        glm::ivec3 temp_coordinates=coordinates;
        glm::ivec3 temp_box = m_size;
        rinsert(m_root, &data, &temp_coordinates, &temp_box);
    }

    const glm::ivec3 size() { return m_size;}
    const uint32_t cell_count() { return m_nodecount*8-m_leafcount*7; }
    glm::u8vec4 *serialize()
    {
        if (m_serialized == nullptr)
            delete[] m_serialized;
        m_serialized = new glm::u8vec4[m_nodecount*8-m_leafcount*7];
        uint32_t base_index = 0;
        bool ok = false;
        uint32_t end_index = base_index + 8;
        for (int i = 0; i < 8; i++)
        {
            if (m_root->children[i])
            {
                glm::u32vec2 segment = rserialize(m_root->children[i], end_index, m_serialized);
                m_serialized[base_index + i].x = segment.x >> 16 & 0xFF;
                m_serialized[base_index + i].y = segment.x >> 8 & 0xFF;
                m_serialized[base_index + i].z = segment.x & 0xFF;
                m_serialized[base_index + i].w = 0x00;
                end_index = segment.y + 1;
                ok = true;
            }
            else
            {
                m_serialized[base_index + i] = {0, 0, 0, 0};
            }
        }
        if (!ok)
        {
            m_serialized[base_index] = *(m_root->data);
            end_index = base_index;
        }
        return m_serialized;
    }
    void update()
    {
        glBindBuffer(GL_TEXTURE_BUFFER, TBO);
        glBufferData(GL_TEXTURE_BUFFER, (m_nodecount*8-m_leafcount*7)*sizeof(glm::u8vec4), m_serialized, GL_STATIC_DRAW);
        glBindBuffer(GL_TEXTURE_BUFFER, 0);
    }
    void bind()
    {
        glBindBuffer(GL_TEXTURE_BUFFER, TBO);
        glBindTexture(GL_TEXTURE_BUFFER, TBO_texture);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8UI, TBO);

    }
private:
    void rinsert(octree_node *node, glm::u8vec4 *data, glm::ivec3 *coordinates, glm::ivec3 *box)
    {
        if (box->x == 1)
        {
            node->data = new glm::u8vec4;
            *(node->data) = *data;
            m_leafcount++;
            return;
        }
        unsigned int index = (coordinates->x < box->x / 2) | ((coordinates->y < box->y / 2) << 1) | ((coordinates->z < box->z / 2) << 2);
        coordinates->x -= ((index & 1) != 1) * box->x / 2;
        coordinates->y -= ((index & 2) != 2) * box->y / 2;
        coordinates->z -= ((index & 4) != 4) * box->z / 2;
        if (node->children[index] == nullptr)
        {
            node->children[index] = new octree_node;
            m_nodecount++;
        }
        *box /= 2;
        rinsert(node->children[index], data, coordinates, box);
    }
    glm::u32vec2 rserialize(octree_node *node, uint32_t base_index, glm::u8vec4 *data)
    {
        uint32_t end_index = base_index + 8;
        if (!node->data)
        {
            
            for (int i = 0; i < 8; i++)
            {
                if (node->children[i])
                {
                    glm::u32vec2 segment = rserialize(node->children[i], end_index, data);
                    data[base_index + i].x = segment.x >> 16 & 0xFF;
                    data[base_index + i].y = segment.x >> 8 & 0xFF;
                    data[base_index + i].z = segment.x & 0xFF;
                    data[base_index + i].w = 0x00;
                    end_index = segment.y + 1;
                }
                else
                {
                    data[base_index + i] = {0, 0, 0, 0};
                }
            }
        }else
        {
            data[base_index] = *(node->data);
            end_index = base_index+1;
        }

        return {base_index, end_index-1};
    }
    octree_node *m_root;
    glm::ivec3 m_size;
    glm::u8vec4 *m_serialized = nullptr;
    GLuint TBO, TBO_texture;
    uint32_t m_nodecount;
    uint32_t m_leafcount;
};
#endif