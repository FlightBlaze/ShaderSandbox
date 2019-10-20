#pragma once

#include <wayland-server.h>

class GLFWwindow;

namespace sandbox {

class Demo {
    
    int m_argc;
    char **m_argv;
    GLFWwindow* m_window = 0;
    
    unsigned loadImage(const char*);
    unsigned createTexture(int, int);
    
    void recreateSquareFramebuffer(unsigned&, unsigned&, int, int);
    void recreateFramebuffer(unsigned&, unsigned&, int, int);
    
public:

	static const int initialWidth = 700;
	static const int initialHeight = 500;
    
	unsigned cliImageBuffer[initialWidth * initialHeight];

    Demo(int, char**);
    ~Demo();

	// wayland things
	
	struct wl_display* wDisplay;
    
    int run();
    
};
    
} // end namespace sandbox

