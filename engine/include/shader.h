#pragma once

#include <string>
#include <unordered_map>
#include "platform.h"

#ifndef PLATFORM_WEB
#include <glad/glad.h>
#else
#include <GLES3/gl3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderVariable {
    GLint location;
    GLenum type;
    GLint size;
};

class Shader {
private:
    GLuint handle;
    std::unordered_map<std::string, ShaderVariable> uniformVars;
    std::unordered_map<std::string, ShaderVariable> attribs;

    GLuint compileShader(GLenum type, const std::string& filename, const char* source);
public:
    Shader(const std::string& vertFile, const std::string& fragFile);
    ~Shader();

    void use();
    
    GLint getUniformLocation(const char* name) { return glGetUniformLocation(this->handle, name); }

    void loadUniform(const std::string& name, glm::mat3 matrix);
    void loadUniform(const std::string& name, glm::mat4 matrix);
    void loadUniform(const std::string& name, int value); // { glUniform1i(location, value); }
    void loadUniform(const std::string& name, float value); // { glUniform1f(location, value); }
    void loadUniform(const std::string& name, glm::vec2 value); // { glUniform2fv(location, 1, glm::value_ptr(value)); }
    void loadUniform(const std::string& name, glm::vec3 value); // { glUniform3fv(location, 1, glm::value_ptr(value)); }
    void loadUniform(const std::string& name, glm::vec4 value);
    void loadUniformArray(const std::string& name, int count, int* values);
};
