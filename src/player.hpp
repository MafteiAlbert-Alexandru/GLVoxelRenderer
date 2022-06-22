#ifndef PLAYER_HPP
#define PLATER_HPP 1

#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cmath>
class Player
{

public:
    Player(glm::vec3 pos = {0, 0, 0}, glm::vec3 cam = {1, 0, 0}, float speed = 7.5f, float sensitivity = 7.5f) : m_pos(pos), m_cam(cam), m_speed(speed),
                                                                                                                 m_sensitivity(sensitivity)
    {
    }
    void input(GLFWwindow *window)
    {
        static bool debounce;
        static bool windowMaximized;
        static double lastxpos, lastypos;
        double current_time = glfwGetTime();
        double delta_time = current_time - m_last_time;
        m_last_time = current_time;
        float speed = m_speed;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            speed *= 2;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_pos += m_cam * ((float)delta_time) * speed;
            std::cerr << m_pos.x << " " << m_pos.y << " " << m_pos.z;
            std::cerr << '\n' << floor(m_pos.x) << " " << floor(m_pos.y) << " " << floor(m_pos.z) <<'\n';
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_pos -= m_cam * ((float)delta_time) * speed;
            std::cerr << m_pos.x << " " << m_pos.y << " " << m_pos.z;
            std::cerr << '\n' << floor(m_pos.x) << " " << floor(m_pos.y) << " " << floor(m_pos.z) <<'\n';
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_pos += glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_cam)) * ((float)delta_time) * speed;
            std::cerr << m_pos.x << " " << m_pos.y << " " << m_pos.z;
            std::cerr << '\n' << floor(m_pos.x) << " " << floor(m_pos.y) << " " << floor(m_pos.z) <<'\n';
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_pos += glm::normalize(glm::cross(m_cam, glm::vec3(0, 1, 0))) * ((float)delta_time) * speed;
            std::cerr << m_pos.x << " " << m_pos.y << " " << m_pos.z;
            std::cerr << '\n' << floor(m_pos.x) << " " << floor(m_pos.y) << " " << floor(m_pos.z) <<'\n';
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            m_pos += glm::vec3(0, 1, 0) * ((float)delta_time) * speed;
            std::cerr << m_pos.x << " " << m_pos.y << " " << m_pos.z;
            std::cerr << '\n' << floor(m_pos.x) << " " << floor(m_pos.y) << " " << floor(m_pos.z) <<'\n';
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            m_pos += glm::vec3(0, -1, 0) * ((float)delta_time) * speed;
            std::cerr << m_pos.x << " " << m_pos.y << " " << m_pos.z;
            std::cerr << '\n' << floor(m_pos.x) << " " << floor(m_pos.y) << " " << floor(m_pos.z) <<'\n';
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
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
            {
                glfwRestoreWindow(window);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                windowMaximized = false;
            }
        }
        if (windowMaximized)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glm::vec2 delta = {xpos - lastxpos, ypos - lastypos};
            lastxpos = xpos;
            lastypos = ypos;
            if((m_rotation+delta * m_sensitivity * ((float)delta_time)).y > 80 || (m_rotation+delta * m_sensitivity * ((float)delta_time)).y < -80)
                return;
            m_rotation += delta * m_sensitivity * ((float)delta_time);
            m_cam = glm::rotate({1, 0, 0}, -glm::radians(m_rotation.x), glm::vec3(0, 1, 0));
            m_cam = glm::rotate(m_cam, glm::radians(m_rotation.y), glm::normalize(glm::cross({0, 1, 0}, m_cam)));
            std::cerr<<"cam: " << m_rotation.x << " " << m_rotation.y << '\n';
        }
        
    }
    glm::vec3 &getPosition() { return m_pos; }
    glm::vec3 &getCamera() { return m_cam; }

private:
    glm::vec3 m_pos, m_cam;
    glm::vec2 m_rotation;
    double m_last_time;
    float m_speed, m_sensitivity;
};
#endif // PLAYER_HPP