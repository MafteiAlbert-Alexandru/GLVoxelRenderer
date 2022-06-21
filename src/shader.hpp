#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{

public:
    Shader(const char *vertexShaderFileName = NULL, const char *geometryShaderFileName = NULL, const char *fragmentShaderFileName = NULL)
    {

        m_program=glCreateProgram();
        if (vertexShaderFileName != NULL)
        {
            unsigned int vertexShader;
            std::ifstream vertexShaderFile;
            std::stringstream vertexShaderStream;
            std::string vertexShaderCode;
            vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            vertexShaderFile.open(vertexShaderFileName);
            vertexShaderStream << vertexShaderFile.rdbuf();
            vertexShaderCode = vertexShaderStream.str();
            const char * vShaderCode =vertexShaderCode.c_str();
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vShaderCode, NULL);
            glCompileShader(vertexShader);

            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                          << infoLog << std::endl;

            }
            glAttachShader(m_program, vertexShader);

        }
        if (geometryShaderFileName != NULL)
            ;
        if (fragmentShaderFileName != NULL)
        {
            unsigned int fragmentShader;
            std::ifstream fragmentShaderFile;
            std::stringstream fragmentShaderStream;
            std::string fragmentShaderCode;
            fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fragmentShaderFile.open(fragmentShaderFileName);
            fragmentShaderStream << fragmentShaderFile.rdbuf();
            fragmentShaderCode = fragmentShaderStream.str();
            const char* fShaderCode = fragmentShaderCode.c_str();
            fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
            glCompileShader(fragmentShader);

            int success;
            char infoLog[512];
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

            if (!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
  
            }
            glAttachShader(m_program, fragmentShader);
        }
        int success;
        char infoLog[512];
        glLinkProgram(m_program);
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_program, 512, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }
       
    }
    void use()
    {
        glUseProgram(m_program);
    }
    void setInt(std::string name, int value)
    {
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
    }
private:
    unsigned int m_program;
};

#endif