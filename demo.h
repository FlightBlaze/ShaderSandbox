#pragma once

class GLFWwindow;

namespace sandbox {
    
class Demo {
    
    int m_argc;
    char **m_argv;
    GLFWwindow* m_window = 0;
    
    unsigned loadImage(const char*);
    unsigned createTexture(int, int);
    
    void recreateFramebuffer(unsigned&, unsigned&, int, int);
    
public:
    
    Demo(int, char**);
    ~Demo();
    
    int run();
    
};
    
} // end namespace sandbox

