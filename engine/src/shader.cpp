#include <shader.h>

#include <stdexcept>

#include <file_io.h>
#include <log.h>

#define MAX_BUF_SIZE 256

const char* translateGLEnum(GLenum value) {
	switch (value) {
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_INT:
		return "int";
	case GL_INT_VEC2:
		return "ivec2";
	case GL_INT_VEC3:
		return "ivec3";
	case GL_INT_VEC4:
		return "ivec4";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_UNSIGNED_INT_VEC2:
		return "uvec2";
	case GL_UNSIGNED_INT_VEC3:
		return "uvec3";
	case GL_UNSIGNED_INT_VEC4:
		return "uvec4";
	case GL_BOOL:
		return "bool";
	case GL_BOOL_VEC2:
		return "bvec2";
	case GL_BOOL_VEC3:
		return "bvec3";
	case GL_BOOL_VEC4:
		return "bvec4";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	case GL_FLOAT_MAT2x3:
		return "mat2x3";
	case GL_FLOAT_MAT2x4:
		return "mat2x4";
	case GL_FLOAT_MAT3x2:
		return "mat3x2";
	case GL_FLOAT_MAT3x4:
		return "mat3x4";
	case GL_FLOAT_MAT4x2:
		return "mat4x2";
	case GL_FLOAT_MAT4x3:
		return "mat4x3";
	case GL_SAMPLER_1D:
		return "sampler1D";
	case GL_SAMPLER_2D:
		return "sampler2D";
	case GL_SAMPLER_3D:
		return "sampler3D";
	case GL_SAMPLER_CUBE:
		return "samplerCube";
	case GL_SAMPLER_1D_SHADOW:
		return "sampler1DShadow";
	case GL_SAMPLER_2D_SHADOW:
		return "sampler2DShadow";
	case GL_SAMPLER_1D_ARRAY:
		return "sampler1DArray";
	case GL_SAMPLER_2D_ARRAY:
		return "sampler2DArray";
	case GL_SAMPLER_1D_ARRAY_SHADOW:
		return "sampler1DArrayShadow";
	case GL_SAMPLER_2D_ARRAY_SHADOW:
		return "sampler2DArrayShadow";
	case GL_SAMPLER_2D_MULTISAMPLE:
		return "sampler2DMS";
	case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "sampler2DMSArray";
	case GL_SAMPLER_CUBE_SHADOW:
		return "samplerCubeShadow";
	case GL_SAMPLER_BUFFER:
		return "samplerBuffer";
	case GL_SAMPLER_2D_RECT:
		return "sampler2DRect";
	case GL_SAMPLER_2D_RECT_SHADOW:
		return "sampler2DRectShadow";
	case GL_INT_SAMPLER_1D:
		return "isampler1D";
	case GL_INT_SAMPLER_2D:
		return "isampler2D";
	case GL_INT_SAMPLER_3D:
		return "isampler3D";
	case GL_INT_SAMPLER_CUBE:
		return "isamplerCube";
	case GL_INT_SAMPLER_1D_ARRAY:
		return "isampler1DArray";
	case GL_INT_SAMPLER_2D_ARRAY:
		return "isampler2DArray";
	case GL_INT_SAMPLER_2D_MULTISAMPLE:
		return "isampler2DMS";
	case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "isampler2DMSArray";
	case GL_INT_SAMPLER_BUFFER:
		return "isamplerBuffer";
	case GL_INT_SAMPLER_2D_RECT:
		return "isampler2DRect";
	case GL_UNSIGNED_INT_SAMPLER_1D:
		return "usampler1D";
	case GL_UNSIGNED_INT_SAMPLER_2D:
		return "usampler2D";
	case GL_UNSIGNED_INT_SAMPLER_3D:
		return "usampler3D";
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
		return "usamplerCube";
	case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
		return "usampler2DArray";
	case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
		return "usampler2DArray";
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
		return "usampler2DMS";
	case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
		return "usampler2DMSArray";
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		return "usamplerBuffer";
	case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
		return "usampler2DRect";
	default:
		return "<missing-type>";
	}
}

Shader::Shader(const std::string& vertFile, const std::string& fragFile) : uniformVars(), attribs() {
	std::string vertSrc = read_file_str(vertFile);
	std::string fragSrc = read_file_str(fragFile);

	// prepend the correct version to the beginning of the source file
#ifdef PLATFORM_WEB
	vertSrc.insert(0, "#version 300 es\n");
	fragSrc.insert(0, "#version 300 es\n");
#else
	vertSrc.insert(0, "#version 330 core\n");
	fragSrc.insert(0, "#version 330 core\n");
#endif

	Log::getRendererLog()->trace("Transformed vertex shader!");
	Log::getRendererLog()->trace("Transformed fragment shader!");

	GLuint vertObj = compileShader(GL_VERTEX_SHADER, vertFile, vertSrc.c_str());
	GLuint fragObj = compileShader(GL_FRAGMENT_SHADER, fragFile, fragSrc.c_str());

	// delete[] vertSrc;
	// delete[] fragSrc;

	this->handle = glCreateProgram();
	glAttachShader(this->handle, vertObj);
	glAttachShader(this->handle, fragObj);

	glLinkProgram(this->handle);

	GLint success;
	glGetProgramiv(this->handle, GL_LINK_STATUS, &success);

	if (!success) {
		Log::getRendererLog()->error("Error whilst linking shader program (vertex filename={}, fragment filename={})", vertFile, fragFile);

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

	// lets read through uniforms
	GLint uniformCount = 0;
	glGetProgramiv(this->handle, GL_ACTIVE_UNIFORMS, &uniformCount);

	GLint longestUniformLength = 0;
	glGetProgramiv(this->handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &longestUniformLength);

	Log::getRendererLog()->trace("Shader has {} active uniforms named:", uniformCount);
	for (GLint i = 0; i < uniformCount; i++) {
		GLchar* uniformName = new GLchar[longestUniformLength]{ };
		GLsizei uniformNameLength = 0;
		GLint uniformSize = 0;
		GLenum uniformType = 0;
		glGetActiveUniform(this->handle, (GLuint)i, longestUniformLength, &uniformNameLength, &uniformSize, &uniformType, uniformName);
		GLint uniformLocation = getUniformLocation(uniformName);

		Log::getRendererLog()->trace("[{}] \"{}\": {} x {}, loc={}", i, uniformName, translateGLEnum(uniformType), uniformSize, uniformLocation);

		this->uniformVars.emplace(std::make_pair(std::string(uniformName), ShaderVariable{ uniformLocation, uniformType, uniformSize }));
	}

	// lets now read through the attribs
	GLint attribCount = 0;
	glGetProgramiv(this->handle, GL_ACTIVE_ATTRIBUTES, &attribCount);

	GLint longestAttribLength = 0;
	glGetProgramiv(this->handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &longestAttribLength);

	Log::getRendererLog()->trace("Shader has {} active attribs:", attribCount);
	for (GLint i = 0; i < attribCount; i++) {
		GLchar* attribName = new GLchar[longestAttribLength]{ };
		GLsizei attribNameLength = 0;
		GLint attribSize = 0;
		GLenum attribType = 0;
		glGetActiveAttrib(this->handle, (GLuint)i, longestAttribLength, &attribNameLength, &attribSize, &attribType, attribName);
		GLint attribLocation = glGetAttribLocation(this->handle, attribName);

		Log::getRendererLog()->trace("[{}] \"{}\": {} x {}, loc={}", i, attribName, translateGLEnum(attribType), attribSize, attribLocation);

		this->attribs.emplace(std::make_pair(std::string(attribName), ShaderVariable{ attribLocation, attribType, attribSize }));
	}
}

Shader::~Shader() {
	glDeleteProgram(this->handle);
}

void Shader::use() {
	glUseProgram(this->handle);
}

GLuint Shader::compileShader(GLenum type, const std::string& filename, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// check for compilation issues
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		switch (type) {
		case GL_VERTEX_SHADER:
			Log::getRendererLog()->error("Error compiling vertex shader! (filename={})", filename);
			break;
		case GL_FRAGMENT_SHADER:
			Log::getRendererLog()->error("Error compiling fragment shader! (filename={})", filename);
			break;
		default:
			Log::getRendererLog()->error("Error compiling <missing-type> shader! (filename={})", filename);
			break;
		}

		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		GLchar* buffer = new GLchar[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, buffer);
		Log::getRendererLog()->error(buffer);
		delete[] buffer;

		throw std::runtime_error("Shader compilation failed!");
	}

	return shader;
}

void Shader::loadUniform(const std::string& name, glm::mat3 matrix) {
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}
	
	ShaderVariable uniform = this->uniformVars.at(name);

	if (uniform.type != GL_FLOAT_MAT3 || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not a mat4 variable that exists on this shader.");
	}
	
	glUniformMatrix3fv(uniform.location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::loadUniform(const std::string& name, glm::mat4 matrix) {
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name);

	if (uniform.type != GL_FLOAT_MAT4 || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not a mat4 variable that exists on this shader.");
	}

	glUniformMatrix4fv(uniform.location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::loadUniform(const std::string& name, int value) {
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name);

	if (!(uniform.type == GL_INT || uniform.type == GL_SAMPLER_2D) || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not an int variable that exists on this shader.");
	}

	glUniform1i(uniform.location, value);
}

void Shader::loadUniform(const std::string& name, float value) {
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name);

	if (uniform.type != GL_FLOAT || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not an float variable that exists on this shader.");
	}

	glUniform1f(uniform.location, value);
}

void Shader::loadUniform(const std::string& name, glm::vec2 value) {
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name);

	if (uniform.type != GL_FLOAT_VEC2 || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not an vec2 variable that exists on this shader.");
	}

	glUniform2fv(uniform.location, 1, glm::value_ptr(value));
}

void Shader::loadUniform(const std::string& name, glm::vec3 value) {
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name);

	if (uniform.type != GL_FLOAT_VEC3 || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not an vec3 variable that exists on this shader.");
	}

	glUniform3fv(uniform.location, 1, glm::value_ptr(value));
}

void Shader::loadUniform(const std::string& name, glm::vec4 value)
{
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name);

	if (uniform.type != GL_FLOAT_VEC4 || uniform.size != 1) {
		throw std::invalid_argument("Error: " + name + " is not an vec4 variable that exists on this shader.");
	}

	glUniform4fv(uniform.location, 1, glm::value_ptr(value));
}

void Shader::loadUniformArray(const std::string& name, int count, int* values) { 
	if (this->uniformVars.count(name) <= 0) {
		throw std::invalid_argument("Error: The uniform variable " + name + " does not exist on this shader.");
	}

	ShaderVariable uniform = this->uniformVars.at(name); 

	// write check for var type

	glUniform1iv(uniform.location, count, values); 
};
