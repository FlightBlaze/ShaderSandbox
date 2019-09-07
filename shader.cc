#include "shader.h"
#include <iostream>

namespace sandbox {

Shader :: Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    // Get sources
    std::ifstream vShaderFile, fShaderFile;
    std::string vertexCode, fragmentCode;
    
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();     
    }
    catch(std::ifstream::failure e) {
        std::cout << "File read error" << std::endl;
    }
    
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    
    GLuint vert, frag;
    GLint success;
    GLchar infoLog[512];

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vShaderCode, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        std::cout << "Vertex shader compile time error\n" << infoLog << std::endl;
    }
    
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        std::cout << "Fragment shader compile time error\n" << infoLog << std::endl;
    }    
    
    this->program = glCreateProgram();
    glAttachShader(this->program, vert);
    glAttachShader(this->program, frag);
    glLinkProgram(this->program);
    
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "Linking error\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader :: use() {
    glUseProgram(this->program);
}  

} // end of namespace sandbox
