#include "ShaderProgram.h"
#include "ContentManager.h"
#include "Uniforms.h"
#include "Logger.h"

#include <sstream>
#include <regex>

using namespace std;
using namespace fmt;
using namespace glm;

const regex ShaderProgram::DIRECTIVE_REGEX(R"(^#pragma (.*))");
const regex ShaderProgram::INCLUDE_REGEX(R"(include (?:<(.*)>|'(.*)'|\"(.*)\"))");
const regex ShaderProgram::LINE_NUMBER_REGEX(R"(^([\d+])\((\d+)\))");

ShaderProgram::ShaderProgram(): m_program(0), m_invalidUniform(false) { }

ShaderProgram::~ShaderProgram() {
	DeleteShaders();

	if (m_program) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

bool ShaderProgram::Init() {
	m_program = glCreateProgram();

	if (!m_program) {
		Logger::Console()->warn("Could not create shader program.");
		return false;
	}

	return true;
}

void ShaderProgram::Enable() const {
#ifdef _DEBUG
	if (!m_program) {
		Logger::Console()->warn("Using uninitialized shader program.");
	}
#endif
	glUseProgram(m_program);
}

void ShaderProgram::Disable() {
	glUseProgram(0);
}

bool ShaderProgram::AddShader(const GLenum a_shaderType, const string& a_filePath) {
	// Try getting the source from the file
	ShaderSource shaderSource = GetShaderSource(a_filePath);
	if (!shaderSource.m_valid) return false;

	// Try to create a shader object
	const GLuint shader = glCreateShader(a_shaderType);
	if (!shader) {
		Logger::Console()->warn("Could not create shader of type {}", a_shaderType);
		return false;
	}

	// Save the shader for future linking
	m_shaders.push_back(shader);

	// Attach and compile the source to the shader
	const GLchar *sourcePointer = shaderSource.m_source.c_str();
	glShaderSource(shader, 1, &sourcePointer, nullptr);
	glCompileShader(shader);

	// Check compile status and print compilation errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shader, info.length(), &length, &info[0]);

		smatch matches;
		string error = info;
		ShaderSource& errorSource = shaderSource;
		if (regex_search(info, matches, LINE_NUMBER_REGEX)) {
			// Get line and column numbers from error message
			size_t lineNumber = stoi(matches[2], nullptr);
			const size_t columnNumber = stoi(matches[2], nullptr);

			// Get the shader source which the error occurred in and update the line number
			errorSource = GetShaderSourceByLineNumber(shaderSource, lineNumber);
			lineNumber -= errorSource.m_start;

			// Construct the error message with the new line number
			error = format("{}({}){}", lineNumber, columnNumber, matches.suffix().str());
		}

		// Log the error message
		Logger::Console()->warn("Could not compile shader {}: {}", errorSource.m_resolvedFilePath, error);
		Logger::File()->warn("Could not compile shader {}:\n\n{}\n{}", errorSource.m_resolvedFilePath, errorSource.m_source, error);

		return false;
	}

	// Attach the shader to our shader program
	glAttachShader(m_program, shader);

	return true;
}

bool ShaderProgram::Finalize() {
	// Try linking the program
	glLinkProgram(m_program);

	// Check link status and print errors
    GLint status, length;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
        string info(length, ' ');
        glGetProgramInfoLog(m_program, info.length(), &length, &info[0]);
		Logger::Console()->warn("Could not link shader program:\n{}", info);
    }

	// Validate the program
	glValidateProgram(m_program);

	// Check validate status and print errors
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
        string info(length, ' ');
        glGetProgramInfoLog(m_program, info.length(), &length, &info[0]);
		Logger::Console()->warn("Invalid shader program:\n{}", info);
    }

	// Delete shaders because we no longer need them
	DeleteShaders();

	return glGetError() == GL_NO_ERROR;
}

uniform_loc ShaderProgram::GetUniformLocation(const string& a_uniformName) {
	const uniform_loc location = glGetUniformLocation(m_program, a_uniformName.c_str());
	if (location == INVALID_UNIFORM_LOCATION) {
		m_invalidUniform = true;
		Logger::Console()->warn("Invalid uniform \"{}\"", a_uniformName);
	}
	return location;
}

GLint ShaderProgram::GetProgramParam(GLint a_param) const {
	GLint value;
	glGetProgramiv(m_program, a_param, &value);
	return value;
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const bool& a_value) {
	glUniform1ui(a_location, a_value);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const GLuint& a_value) {
	glUniform1i(a_location, a_value);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const int& a_value) {
	glUniform1i(a_location, a_value);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const float& a_value) {
	glUniform1f(a_location, a_value);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const vec2& a_value) {
	glUniform2f(a_location, a_value.x, a_value.y);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const vec3& a_value) {
	glUniform3f(a_location, a_value.x, a_value.y, a_value.z);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const vec4& a_value) {
	glUniform4f(a_location, a_value.x, a_value.y, a_value.z, a_value.w);
}

void ShaderProgram::LoadUniform(const uniform_loc& a_location, const mat4& a_value) {
	glUniformMatrix4fv(a_location, 1, false, &a_value[0][0]);
}

const ShaderSource& ShaderProgram::
GetShaderSourceByLineNumber(const ShaderSource& a_shaderSource, size_t a_lineNumber) {
	for (const ShaderSource& include : a_shaderSource.m_includes) {
		// Check if the line number is in the range of the include
		if (include.m_start <= a_lineNumber && include.m_start + include.m_lines - 1 >= a_lineNumber) {
			return GetShaderSourceByLineNumber(include, a_lineNumber);
		}
	}
	return a_shaderSource;
}

ShaderSource ShaderProgram::GetShaderSource(const string& a_filePath) const {
	// Define invalid shader source struct
	ShaderSource shaderSource;

	// List roots to look in (TODO: Relative to "parent")
	const std::string content = ContentManager::GetContentPath("", "Shaders/");
	const string roots[] = { content, content + "Include/" };

	// Attempt to read the source
	istringstream sourceStream;
	bool success = false;
	for (const std::string& root : roots) {
		shaderSource.m_resolvedFilePath = root + a_filePath;
		string source;
		success = ContentManager::ReadFile(shaderSource.m_resolvedFilePath, source);
		if (success) {
			sourceStream = istringstream(source);
			break;
		}
	}

	// If no source was found, log and return
	if (!success) {
		Logger::Console()->warn("No shader was found with relative path: {}", a_filePath);
		return shaderSource;
	}

	// Search for preprocessor directives
	stringstream source;
	for (string line; getline(sourceStream, line); ) {
		// Check if line is a directive
		smatch matches;
		if (regex_match(line, matches, DIRECTIVE_REGEX)) {
			// Get the directive argument (everything after '#pragma ')
			const string& arg = matches[1];

			// Handle include directive
			smatch argMatches;
			if (regex_match(arg, argMatches, INCLUDE_REGEX)) {
				// Different capture group for '', "", <>. One group will contain path, other two will be empty string
				const string includePath = argMatches[1].str() + argMatches[2].str() + argMatches[3].str();

				// Load the include source
				ShaderSource include = GetShaderSource(includePath);
				if (!include.m_valid) return shaderSource;

				// Mark the start line of the include
				include.m_start = shaderSource.m_lines;

				// Add the include to our source
				shaderSource.m_lines += include.m_lines;
				shaderSource.m_includes.push_back(include);
				source << include.m_source;
			}

			// Handle invalid directive
			else {
				Logger::Console()->warn("Unknown #pragma directive: \"{}\" in shader file {}", arg, shaderSource.m_resolvedFilePath);
				return shaderSource;
			}
		}

		// Add non-directive lines to our source
		else {
			shaderSource.m_lines++;
			source << line << endl;
		}
	}

	// Return valid source
	shaderSource.m_source = source.str();
	// Logger::Console()->info("Preprocessed shader source:\n\n{}", shaderSource.m_source);
	// Logger::File()->info("Preprocessed shader source:\n\n{}", shaderSource.m_source);
	shaderSource.m_valid = true;
	return shaderSource;
}

void ShaderProgram::DeleteShaders() {
	for (GLuint shader : m_shaders) {
		glDeleteShader(shader);
	}
	m_shaders.clear();
}
