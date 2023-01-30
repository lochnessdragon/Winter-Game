#include <shader.h>

#include <stdexcept>

#include <glad/glad.h>

#include <file_io.h>
#include <log.h>
#include <platform.h>

Shader::Shader(const std::string& vertFile, const std::string& fragFile) {
    std::string vertSrc = std::string(read_file(vertFile));
    std::string fragSrc = std::string(read_file(fragFile));

	// prepend the correct version to the beginning of the source file
	#ifdef PLATFORM_WEB
	vertSrc.insert(0, "#version 320 es\n");
	fragSrc.insert(0, "#version 320 es\n");
	#else
	vertSrc.insert(0, "#version 330 core\n");
	fragSrc.insert(0, "#version 330 core\n");
	#endif

	Log::getRendererLog()->info("Transformed vertex shader: {}", vertSrc);
	Log::getRendererLog()->info("Transformed fragment shader: {}", fragSrc);

    GLuint vertObj = compileShader(GL_VERTEX_SHADER, vertSrc.c_str());
    GLuint fragObj = compileShader(GL_FRAGMENT_SHADER, fragSrc.c_str());

    // delete[] vertSrc;
    // delete[] fragSrc;

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