#include"Texture.h"
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_img/stb_image.h>

Texture::Texture(GLenum textureTarget, const std::string& filename) {
	m_textureTarget = textureTarget;
	m_filename = filename;
}

// should be called once to load the texture
bool Texture::Load(){
	// since stbi load from top to bottom, we need to flip it to get bottom to top as in opengl
	stbi_set_flip_vertically_on_load(1);
	int width, height, bpp; // bits per pixel
	// set to 0 for loading all the channels
	unsigned char* imageData = stbi_load(m_filename.c_str(), &width, &height, &bpp, 0);

	if (!imageData) {
		printf("Can't load texture %s - %s\n", m_filename.c_str(), stbi_failure_reason());
		exit(0);
	}
	printf("Width : %d Height : %d bpp : %d\n", width, height, bpp);

	glGenTextures(1, &m_textureObject);
	glBindTexture(m_textureTarget, m_textureObject);
	if (m_textureTarget == GL_TEXTURE_2D) {
		// load tex data to tex obj
		// mip level howToStoreInGPU formatOfOrginalImage
		glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	else {
		std::cout << "Not implemented the type till now" << std::endl;
		exit(1);
	}

	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// unbind 
	glBindTexture(m_textureTarget, 0);
	// free
	stbi_image_free(imageData);

	return true;
}

// must be called once to for specific texture unit
void Texture::Bind(GLenum textureUnit){
	glActiveTexture(textureUnit);
	glBindTexture(m_textureTarget, m_textureObject);
}