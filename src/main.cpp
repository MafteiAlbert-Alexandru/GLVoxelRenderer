#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "shader.hpp"
#include "screen.hpp"
#include "texture3d.hpp"
#include "worldgen.hpp"
#include <iostream>
#include <glm/geometric.hpp>
#include <cstdlib>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
glm::ivec2 screen_size = {800, 600};
glm::vec3 pos = {32, 32, 32}, cam = {0, 0, 1};
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_size = {width, height};
}
bool windowMaximized = false, debounce = false;
double last_time;
double lastxpos, lastypos;
glm::vec2 rotation;
void processInput(GLFWwindow *window)
{
    double current_time=glfwGetTime();
    double delta_time=current_time-last_time;
    last_time = current_time;
    float speed =7.5f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed=15.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        pos += cam*((float)delta_time)*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        pos -= cam*((float)delta_time)*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        pos += glm::normalize(glm::cross(glm::vec3(0,1,0), cam))*((float)delta_time)*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        pos += glm::normalize(glm::cross(cam, glm::vec3(0,1,0)))*((float)delta_time)*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        pos += glm::vec3(0, 1, 0)*((float)delta_time)*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        pos += glm::vec3(0, -1, 0)*((float)delta_time)*speed;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        debounce = true;
    }
    if (debounce && glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        debounce = false;
        if (!windowMaximized)
        {
            glfwMaximizeWindow(window);
            windowMaximized = true;
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwRestoreWindow(window);
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            windowMaximized = false;
        }
    }
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glm::vec2 delta ={xpos-lastxpos, ypos-lastypos};
    lastxpos=xpos;
    lastypos=ypos;
    std::cerr << delta. x<< " " << delta.y<<'\n';
    rotation+=delta*speed*((float)delta_time);
    cam =glm::rotate({1,0,0}, -glm::radians(rotation.x), glm::vec3(0,1,0));
    cam =glm::rotate(cam, glm::radians(rotation.y), glm::normalize(glm::cross({0,1,0}, cam)));
}
//float last_time;



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
    Texture3D texture(64, 64, 64);
    Screen screen(&texture);
    screen.setShader(&baseShader);
    WorldGen::generate(&texture, 0.7, 6);
    baseShader.use();
    baseShader.setInt("tex_input", 0);
    screen.m_texture->update();
    int i = 0;
    glfwSetTime(0.0);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        baseShader.setFloat3("pos", pos);
        baseShader.setFloat3("cam", cam);
        baseShader.setInt("FOV", 90);
        baseShader.setInt2("screen_size", screen_size);
        baseShader.setInt3("map_size", texture.size());
        screen.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}