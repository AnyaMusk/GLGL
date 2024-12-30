#pragma once

#include<string>
#include<glad/glad.h>

class Texture {
public:
	Texture(GLenum textureTarget, const std::string& filename);
	
	// should be called once to load the texture
	bool Load();

	// must be called once to for specific texture unit
	void Bind(GLenum textureUnit);

private:
	std::string m_filename;
	GLenum m_textureTarget;
	unsigned int m_textureObject;
};