#include "Logger.h"

using namespace std;
using namespace spdlog;

shared_ptr<logger> Logger::s_console = stdout_color_mt("console");
shared_ptr<logger> Logger::s_file = basic_logger_mt("file", "log.txt");

void Logger::GlErrorCallback(GLenum a_source, GLenum a_type, GLuint a_id, GLenum a_severity, GLsizei a_length,
                             const GLchar* a_message, const void* a_userParam) {

	const auto severity = GetSeverity(a_severity);
	// if (severity == level::err) return;
	Console()->log(severity, a_message);
}

const shared_ptr<logger>& Logger::Console() {
	return s_console;
}

const std::shared_ptr<spdlog::logger>& Logger::File() {
	return s_file;
}

string Logger::GetSource(GLenum a_source) {
	string source;
	switch (a_source) {
	case GL_DEBUG_SOURCE_API:
		source = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		source = "WINDOW SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		source = "SHADER COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		source = "THIRD PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		source = "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
	default:
		source = "UNKNOWN";
	}
	return source;
}

string Logger::GetType(GLenum a_type) {
	string type;
	switch (a_type) {
	case GL_DEBUG_TYPE_ERROR:
		type = "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		type = "DEPRECATED BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		type = "UDEFINED BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		type = "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		type = "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		type = "OTHER";
		break;
	case GL_DEBUG_TYPE_MARKER:
		type = "MARKER";
		break;
	default:
		type = "UNKNOWN";
	}
	return type;
}

level::level_enum Logger::GetSeverity(GLenum a_severity) {
	level::level_enum severity;
	switch (a_severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		severity = level::critical;
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		severity = level::err;
		break;
	case GL_DEBUG_SEVERITY_LOW:
		severity = level::warn;
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
	default:
		severity = level::info;
	}
	return severity;
}
