#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShaderProgram
{
private:
    unsigned int m_id;

    void _createShader(const char* shaderSource, GLenum SHADER_TYPE, unsigned int& vertexShader);
    void _creteShaderProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int& outShaderProgram);

    void _checkShaderCompilation(unsigned int& shader, GLenum SHADER_TYPE);
    void _checkShaderProgramCompilation(unsigned int& shaderProgram);
public:
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

    ~ShaderProgram()
    {
        if (m_id) glDeleteProgram(m_id);
    }

    void use() const;
    unsigned int getId() const;

    // TODO: handle errors if cannot set to uniform with glGetError()
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, glm::vec3 value) const;
    void setUniform(const std::string& name, glm::mat4 value) const;
};
