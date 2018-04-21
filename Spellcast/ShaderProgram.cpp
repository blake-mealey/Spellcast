#include "ShaderProgram.h"
#include "ContentManager.h"
#include "Uniforms.h"

#include <iostream>
#include <regex>

using namespace std;
using namespace glm;

const regex ShaderProgram::S_NEW_LINE(R"(\/\/.*)");
const regex ShaderProgram::S_DIRECTIVE(R"(^#pragma (.*))");
const regex ShaderProgram::S_INCLUDE(R"(include (?:<(.*)>|'(.*)'|\"(.*)\"))");

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
		cerr << "WARNING: Could not create shader program." << endl;
		return false;
	}

	return true;
}

void ShaderProgram::Enable() const {
#ifdef _DEBUG
	if (!m_program) {
		cerr << "WARNING: Using uninitialized shader program." << endl;
	}
#endif
	glUseProgram(m_program);
}

void ShaderProgram::Disable() {
	glUseProgram(0);
}

bool ShaderProgram::AddShader(const GLenum a_shaderType, const string& a_filePath) {
	// Try reading source from file
	string source, resolvedFilePath;
	if (!PreprocessShaderSource(a_filePath, source, resolvedFilePath)) {
		return false;
	}

	// Try to create a shader object
	const GLuint shader = glCreateShader(a_shaderType);
	if (!shader) {
		cerr << "WARNING: Could not create shader of type " << a_shaderType << endl;
		return false;
	}

	// Save the shader for future linking
	m_shaders.push_back(shader);

	// Attach and compile the source to the shader
	const GLchar *sourcePointer = source.c_str();
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
		cerr << "WARNING: Could not compile shader " << resolvedFilePath << ":" << endl << endl << source << endl << info << endl;
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
        cerr << "WARNING: Could not link shader program:" << endl << info << endl;
    }

	// Validate the program
	glValidateProgram(m_program);

	// Check validate status and print errors
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
        string info(length, ' ');
        glGetProgramInfoLog(m_program, info.length(), &length, &info[0]);
        cerr << "WARNING: Invalid shader program:" << endl << info << endl;
    }

	// Delete shaders because we no longer need them
	DeleteShaders();

	return glGetError() == GL_NO_ERROR;
}

GLint ShaderProgram::GetUniformLocation(const string& a_uniformName) {
	const GLuint location = glGetUniformLocation(m_program, a_uniformName.c_str());
	if (location == INVALID_UNIFORM_LOCATION) {
		m_invalidUniform = true;
		cerr << "WARNING: Invalid uniform " << a_uniformName << endl;
	}
	return location;
}

GLint ShaderProgram::GetProgramParam(GLint a_param) const {
	GLint value;
	glGetProgramiv(m_program, a_param, &value);
	return value;
}

void ShaderProgram::LoadUniform(const GLint& a_location, const bool& a_value) {
	glUniform1ui(a_location, a_value);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const GLuint& a_value) {
	glUniform1ui(a_location, a_value);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const int& a_value) {
	glUniform1i(a_location, a_value);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const float& a_value) {
	glUniform1f(a_location, a_value);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const vec2& a_value) {
	glUniform2f(a_location, a_value.x, a_value.y);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const vec3& a_value) {
	glUniform3f(a_location, a_value.x, a_value.y, a_value.z);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const vec4& a_value) {
	glUniform4f(a_location, a_value.x, a_value.y, a_value.z, a_value.w);
}

void ShaderProgram::LoadUniform(const GLint& a_location, const mat4& a_value) {
	glUniformMatrix4fv(a_location, 1, false, &a_value[0][0]);
}

bool ShaderProgram::PreprocessShaderSource(const string& a_filePath, string& a_source, string& a_resolvedFilePath) const {
	// Get the content path for the file
	a_resolvedFilePath = ContentManager::GetContentPath(a_filePath, "Shaders/");

	// TODO: Try different directories
	// Load the initial source
	string searchSource;
	if (!ContentManager::ReadFile(a_resolvedFilePath, searchSource)) {
		ContentManager::NoFileWarning("shader", a_resolvedFilePath.c_str());
		return false;
	}

	// Search for preprocessor directives
	smatch matches;
	stringstream source;
	while (regex_search(searchSource, matches, S_DIRECTIVE)) {
		const string& arg = matches[1];
		smatch argMatches;

		// Handle include directive
		if (regex_match(arg, argMatches, S_INCLUDE)) {
			// Different capture group for '', "", <>. One group will contain path, other two will be empty string
			const string includePath = argMatches[1].str() + argMatches[2].str() + argMatches[3].str();
			string includeSource;
			if (!PreprocessShaderSource(includePath, includeSource)) return false;
			
			// Flatten before including so we don't break line numbers
			// TODO: Properly keep track of lines and report correct file name and line number in error to cerr
			FlattenSource(includeSource);
			source << matches.prefix() << includeSource;
		}

		// Move the search source ahead
		searchSource = matches.suffix().str();
	}

	// Append the remaining source
	source << searchSource << endl;

	// Set the return value and return success
	a_source = source.str();
	return true;
}

bool ShaderProgram::PreprocessShaderSource(const string& a_filePath, string& a_source) const {
	string resolvedFilePath;
	return PreprocessShaderSource(a_filePath, a_source, resolvedFilePath);
}

void ShaderProgram::FlattenSource(std::string& a_source) {
	// Remove comments
	string searchSource = a_source;
	smatch matches;
	stringstream sourceStream;
	while (regex_search(searchSource, matches, S_NEW_LINE)) {
		sourceStream << matches.prefix();
		searchSource = matches.suffix().str();
	}
	sourceStream << searchSource << endl;
	a_source = sourceStream.str();
	
	// Remove new lines
	a_source.erase(std::remove(a_source.begin(), a_source.end(), '\n'), a_source.end());
	a_source.erase(std::remove(a_source.begin(), a_source.end(), '\r'), a_source.end());
}

void ShaderProgram::DeleteShaders() {
	for (GLuint shader : m_shaders) {
		glDeleteShader(shader);
	}
	m_shaders.clear();
}
