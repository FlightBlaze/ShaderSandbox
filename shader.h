#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

namespace sandbox {

struct Shader {
    Shader(const GLchar*, const GLchar*);
    GLuint program;
    void use();
};

} // end of namespace sandbox
