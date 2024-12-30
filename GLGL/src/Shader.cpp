#include "Shader.h"
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    int success;
    char infoLog[512];

    unsigned int vertexShader = CompileShader(vertexFile, GL_VERTEX_SHADER);
    unsigned int fragmentShader = CompileShader(fragmentFile, GL_FRAGMENT_SHADER);

    id = glCreateProgram();

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 512, 0, infoLog);
        std::cout << "Failed in program Linking : " << std::endl << infoLog << std::endl;
    }

    glValidateProgram(id);
    glGetProgramiv(id, GL_VALIDATE_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 512, 0, infoLog);
        std::cout << "Failed in program validation : " << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
unsigned int Shader::CompileShader(const char* filepath, GLenum type) {
    GLuint ret = glCreateShader(type);

    const GLchar* p[1];
    std::string s = LoadShaderSrc(filepath);
    const char* src = s.c_str();

    GLint Lengths[1];
    Lengths[0] = strlen(s.c_str());

   
    glShaderSource(ret, 1, &src, nullptr);
    glCompileShader(ret);

    int success;
    char infoLog[512];

    glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, 0, infoLog);
        std::cout << "Failed in Shader compilation : " << std::endl << infoLog << std::endl;
    }
    return ret;
}

// utils

void Shader::UseProgram() {
    glUseProgram(id);
}

void Shader::DeleteProgram() {
    glDeleteProgram(id);
}

std::string Shader::LoadShaderSrc(const char* filename) {
    std::ifstream file;
    std::string ret;
    std::stringstream buf;
    file.open(filename);

    if (file.is_open()) {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
    {
        std::cout << "Can't open the file : " << filename << std::endl;
    }
    file.close();
    return ret;
}