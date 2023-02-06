#pragma once

#include <string>
#include "platform.h"

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    GLuint handle;

    GLuint compileShader(GLenum type, const char* source);
public:
    Shader(const std::string& vertFile, const std::string& fragFile);
    ~Shader();

    void use();
    
    GLint getUniformLocation(const char* name) { return glGetUniformLocation(this->handle, name); }

    void loadUniform(GLint location, glm::mat4 matrix) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)); }
    void loadUniform(GLint location, float value) { glUniform1f(location, value); }
    void loadUniform(GLint location, int value) { glUniform1i(location, value); }
    void loadUniform(GLint location, glm::vec2 value) { glUniform2fv(location, 1, glm::value_ptr(value)); }
    void loadUniform(GLint location, glm::vec3 value) { glUniform3fv(location, 1, glm::value_ptr(value)); }
};