#include "shaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
    : m_id{}
{
    // Creating of a new vertex shader and store its id
    unsigned int vertexShader;
    unsigned int fragmentShader;

    _createShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader);
    _createShader(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader);

    _creteShaderProgram(vertexShader, fragmentShader, m_id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

unsigned int ShaderProgram::getId() const
{
    return m_id;
}

void ShaderProgram::setUniform(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void ShaderProgram::setUniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setUniform(const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string& name, glm::mat4 value) const
{
    unsigned int location = glGetUniformLocation(m_id, name.c_str());

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::_createShader(const char* shaderSource, GLenum SHADER_TYPE, unsigned int& outShaderObj)
{
    outShaderObj = glCreateShader(SHADER_TYPE);

    glShaderSource(outShaderObj, 1, &shaderSource, NULL);
    glCompileShader(outShaderObj);
    _checkShaderCompilation(outShaderObj, SHADER_TYPE);
}

void ShaderProgram::_creteShaderProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int& outShaderProgram)
{
    outShaderProgram = glCreateProgram();

    glAttachShader(outShaderProgram, vertexShader);
    glAttachShader(outShaderProgram, fragmentShader);
    glLinkProgram(outShaderProgram);
    _checkShaderProgramCompilation(outShaderProgram);
}

void ShaderProgram::_checkShaderCompilation(unsigned int& shader, GLenum SHADER_TYPE)
{
    const int logLength = 512;
    const char* type = SHADER_TYPE == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
    int success;
    char infoLog[logLength];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, logLength, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << '\n';
    }
}

void ShaderProgram::_checkShaderProgramCompilation(unsigned int& shaderProgram)
{
    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << '\n';
    }
}
