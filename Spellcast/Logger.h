#pragma once

#include <spdlog/spdlog.h>
#include <GL/glew.h>

class Logger {
public:
	static void GlErrorCallback(GLenum a_source, GLenum a_type, GLuint a_id,
		GLenum a_severity, GLsizei a_length, const GLchar* a_message, const void* a_userParam);

	static const std::shared_ptr<spdlog::logger>& Console();
	static const std::shared_ptr<spdlog::logger>& File();

private:
	static std::string GetSource(GLenum a_source);
	static std::string GetType(GLenum a_type);
	static spdlog::level::level_enum GetSeverity(GLenum a_severity);

	static std::shared_ptr<spdlog::logger> s_console;
	static std::shared_ptr<spdlog::logger> s_file;
};
