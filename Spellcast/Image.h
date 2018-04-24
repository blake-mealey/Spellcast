#pragma once

#include <GL/glew.h>

#include <string>

typedef unsigned char pixel_t;

class Image {
public:
	Image();
	~Image();

	bool LoadFromFile(const std::string& a_filePath);
	
	pixel_t* GetData() const;
	GLint GetFormat() const;

	int GetWidth() const;
	int GetHeight() const;
private:
	pixel_t* m_data;
	int m_componentCount;

	int m_width;
	int m_height;
};
