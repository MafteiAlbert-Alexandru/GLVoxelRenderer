#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "shader.hpp"
#include "screen.hpp"
#include "texture3d.hpp"
#include "worldgen.hpp"
#include <iostream>
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
    }
}
void GLAPIENTRY
MessageCallback(GLenum source,
                    GLenum type,
                    GLuint id,
                    GLenum severity,
                    GLsizei length,
                    const GLchar *message,
                    const void *userParam)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == 0x824C ? "** GL ERROR **" : ""),
                type, severity, message);
    }
int main(int argc, char *argv[], char *envp[])
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window;
    if ((window = glfwCreateWindow(800, 600, "VoxelRenderer", NULL, NULL)) == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Shader baseShader("shd/shader.vs", NULL, "shd/shader.fs");
    Screen screen;
    screen.setShader(&baseShader);
    WorldGen::generate(screen.m_texture, 0.7, 6);
    baseShader.use();
    baseShader.setInt("tex_input", 0);
    screen.m_texture->update();
    int i=0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        baseShader.setInt("time", i);
        i=time(NULL)%32;
        screen.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    // During init, enable debug output

    glfwTerminate();
    return 0;
}