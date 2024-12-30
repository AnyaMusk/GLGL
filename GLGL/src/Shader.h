#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader {
	public:
		unsigned int id;
		Shader(const char* vertexFile, const char* fragmentFile);
		unsigned int CompileShader(const char* filepath, GLenum type);

		// utils

		void UseProgram();
		void DeleteProgram();
		std::string LoadShaderSrc(const char* filename);
};