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
#include <fstream>
#include "player.hpp"
#include "octree.hpp"
glm::ivec2 screen_size = {800, 600};
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_size = {width, height};
}
bool windowMaximized = false, debounce = false;
double last_time;
double lastxpos, lastypos;

// float last_time;

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
    Player player;
    Shader baseShader("shd/shader.vs", NULL, "shd/shader.fs");
    Octree octree({256, 256, 256});
    Screen screen(&octree);
    screen.setShader(&baseShader);
    baseShader.use();
    baseShader.setInt("tex_input", 0);

    int i = 0;
    glfwSetTime(0.0);
    
    WorldGen::generate(&octree, 0.7, 6);
    //octree.set({255,255,255,255}, {0,0,0});
    glm::u8vec4* data = octree.serialize();
    std::ofstream octree_stream("octree.dat");
    for(int i=0;i<octree.cell_count();i++)
    {
        octree_stream<<(int)data[i].x << " " << (int)data[i].y <<" " << (int)data[i].z << " "<< (int)data[i].w << '\n';
    }
    
    octree_stream.flush();
    octree_stream.close();
    octree.update();
    std::cerr<<octree.cell_count()<<'\n';
    while (!glfwWindowShouldClose(window))
    {
        player.input(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glm::vec3 camera_plan_vector = player.getCamera() * (screen_size.x / 2.0f) * ((float)glm::tan(glm::radians(90 / 2.0)));
        glm::vec3 camera_plan_surface_right_vector = glm::normalize(glm::cross(glm::vec3(0, -1, 0), camera_plan_vector));
        glm::vec3 camera_plan_surface_up_vector = glm::normalize(glm::cross(camera_plan_surface_right_vector, camera_plan_vector));
        baseShader.setFloat3("camera_plan_vector", camera_plan_vector);
        baseShader.setFloat3("camera_plan_surface_right_vector", camera_plan_surface_right_vector);
        baseShader.setFloat3("camera_plan_surface_up_vector", camera_plan_surface_up_vector);
        baseShader.setFloat3("pos", player.getPosition());
        baseShader.setFloat3("cam", player.getCamera());
        baseShader.setInt2("screen_size", screen_size);
        baseShader.setInt3("input_size", octree.size());
        screen.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}