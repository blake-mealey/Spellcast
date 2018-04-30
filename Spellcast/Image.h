#pragma once

#include <GL/glew.h>

#include <string>

typedef unsigned char pixel_t;

class Image {
public:
	Image();
	~Image();

	bool LoadFromFile(const std::string& a_filePath);
	Image* GetSubImage(size_t a_x, size_t a_y, size_t a_width, size_t a_height) const;
	
	pixel_t* GetData() const;
	GLint GetFormat() const;

	int GetWidth() const;
	int GetHeight() const;
private:
	pixel_t* m_data;
	int m_componentCount;

	int m_width;
	int m_height;

	bool m_fromStb;
};
