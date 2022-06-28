#ifndef SCREEN_H
#define SCREEN_H

#include <glad/glad.h>
#include "shader.hpp"
#include "object.hpp"
#include "texture3d.hpp"
#include "octree.hpp"
class Screen: public Object {
    public:
    Screen(Octree * octree)
    {   
        m_octree=octree;
        constexpr float vertices[] = {
            -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f
        };
        constexpr int indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void render()
    {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glActiveTexture(GL_TEXTURE0);
        m_octree->bind();
        m_shader->use();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
    }
    Octree *m_octree;
};

#endif