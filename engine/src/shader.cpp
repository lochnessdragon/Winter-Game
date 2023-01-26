#include <shader.h>

#include <stdexcept>

#include <glad/glad.h>

#include <file_io.h>
#include <log.h>

Shader::Shader(const std::string& vertFile, const std::string& fragFile) {
    char* vertSrc = read_file(vertFile);
    char* fragSrc = read_file(fragFile);

    GLuint vertObj = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint fragObj = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    delete[] vertSrc;
    delete[] fragSrc;

    this->handle = glCreateProgram();
    glAttachShader(this->handle, vertObj);
    glAttachShader(this->handle, fragObj);

    glLinkProgram(this->handle);

    GLint success;
    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);

    if(!success) {
        Log::getRendererLog()->error("Error whilst linking shader program");

        GLint logSize;
        glGetProgramiv(this->handle, GL_INFO_LOG_LENGTH, &logSize);
        GLchar* buffer = new GLchar[logSize];
        glGetProgramInfoLog(this->handle, logSize, &logSize, buffer);
        Log::getRendererLog()->error(buffer);
        delete[] buffer;

        throw std::runtime_error("Failed to link shader program!");
    }

    glDeleteShader(vertObj);
    glDeleteShader(fragObj);
}

Shader::~Shader() {
    glDeleteProgram(this->handle);
}

void Shader::use() {
    glUseProgram(this->handle);
}

GLuint Shader::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // check for compilation issues
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        switch(type) {
        case GL_VERTEX_SHADER:
            Log::getRendererLog()->error("Error compiling vertex shader!");
            break;
        case GL_FRAGMENT_SHADER:
            Log::getRendererLog()->error("Error compiling fragment shader!");
            break;
        default:
            Log::getRendererLog()->error("Error compiling <missing-type> shader!");
            break;
        }

        GLint logSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        GLchar* buffer = new GLchar [logSize];
        glGetShaderInfoLog(shader, logSize, &logSize, buffer);
        Log::getRendererLog()->error(buffer);
        delete[] buffer;

        throw std::runtime_error("Shader compilation failed!");
    }

    return shader;
}