// Important: include GLEW before other graphic libraries
#define GLEW_STATIC
#include <GL/glew.h>

//#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <EGL/egl.h>
//#include <GL/osmesa.h>

#include <GL/gl.h>
//#include <GL/glext.h>

//#include "shader.h"

#include "third_party/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "demo.h"

static
void key_callback(GLFWwindow*, int, int, int, int);
static
void cursor_pos_callback(GLFWwindow*, double, double);

void error_callback(int, const char*);
static
int error(const char*);

static float mouseX = 0.5;
static float mouseY = 0.5;

void GLAPIENTRY
MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(
        stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message
    );
}

namespace sandbox {

GLfloat quadVertices[] = {
    // Position    // UV
   -1.0f,   1.0f,  0.0f, 1.0f,
   -1.0f,  -1.0f,  0.0f, 0.0f,
    1.0f,  -1.0f,  1.0f, 0.0f,

   -1.0f,   1.0f,  0.0f, 1.0f,
    1.0f,  -1.0f,  1.0f, 0.0f,
    1.0f,   1.0f,  1.0f, 1.0f
};

unsigned Demo :: loadImage(const char* path) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    
    unsigned char *image = stbi_load(
        path,
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    
    return texture;
}

unsigned Demo :: createTexture(int width, int height) {
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void Demo :: recreateSquareFramebuffer(unsigned &framebuffer, unsigned &texture, int width, int height) {
    int nwidth = width, nheight = height;
    
    if(width > height)
        nheight *= width / height;
    else
        nwidth *= height / width;
    
    recreateFramebuffer(framebuffer, texture, nwidth, nheight);
}

void Demo :: recreateFramebuffer(unsigned &framebuffer, unsigned &texture, int width, int height) {
    if(framebuffer != 0)
        glDeleteFramebuffers(1, &framebuffer);
    glGenFramebuffers(1, &framebuffer);
    
    if(texture != 0)
        glDeleteTextures(1, &texture);
    texture = createTexture(width, height);
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Demo :: Demo(int argc, char **argv):
    m_argc(), m_argv() {
    
    
}

Demo :: ~Demo() {
}

int Demo :: run() {
	// Check if we are in terminal
    bool cli = true;//!glfwInit();

	if(cli) {
/*		OSMesaContext omcon  = OSMesaCreateContext(OSMESA_RGBA, NULL);
		OSMesaMakeCurrent(
				omcon,
				this->cliImageBuffer,
				GL_UNSIGNED_BYTE,
				this->initialWidth,
				this->initialHeight);*/
	} else { /*
		glfwSetErrorCallback(error_callback);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
		m_window = glfwCreateWindow(
				this->initialWidth,
				this->initialHeight,
				"Demo", 0, 0
		);
	
		glfwSetCursorPosCallback(m_window, cursor_pos_callback);
		glfwSetKeyCallback(m_window, key_callback);
		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);
	*/}

    // Init GLEW
   
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
        return error("Failed to initialize GLEW");
   

    // Enable debug 
    
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    
    // Load shaders
    
    Shader shader(
		"./shaders/sdf.vert",	
        "./shaders/sdf.frag"
    );
    
    // Screen array and buffer

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));   
    
    // Load textures

    GLuint texture0 = loadImage("../shader/resources/background.png");
    GLuint texture1 = loadImage("../shader/resources/photo1.png");
    
    // Main cycle
    
    int rwidth = 0, rheight = 0;
    
    do {
        int width, height;
        
		//glfwGetFramebufferSize(m_window, &width, &height);
        width = this->initialWidth; height = this->initialHeight;
        // Resize textures
        
        if(height != rheight || width != rwidth) {
            rheight = height;
            rwidth = width;
        }
        
        glViewport(0, 0, width, height); 
        
		GLfloat time = 0;//glfwGetTime(); 
       
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);
        
        shader.use();

		glUniform1f(glGetUniformLocation(shader.program, "uTime"), time);

		glUniform2f(glGetUniformLocation(shader.program, "uSize"),
			width, height
		);

		glUniform2f(glGetUniformLocation(shader.program,
			"uMouse"), mouseX, mouseY
		);
		
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUniform1i(glGetUniformLocation(shader.program, "uTexture0"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shader.program, "uTexture1"), 1);
        
        glBindVertexArray(quadVAO);
        
		glDrawArrays(GL_TRIANGLES, 0, 6);
        /*
        glfwSwapBuffers(m_window);
        glfwPollEvents();
        */
    }
    while (true);//(!glfwWindowShouldClose(m_window));
    
    // Ternimate
    
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    /*
    glfwDestroyWindow(m_window);
    glfwTerminate();
    */
    return 0;
}

    
} // end namespace sandbox
/*
void error_callback(int error, const char* description)
 {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_pos_callback(GLFWwindow* w,
		double x, double y) {
	int width, height; 
	glfwGetFramebufferSize(w, &width, &height);
	
	int wx, wy;
	glfwGetWindowPos(w, &wx, &wy);	

	const GLFWvidmode *vidmode = glfwGetVideoMode(
			glfwGetPrimaryMonitor()
	);

	mouseX = (x - wx) / width;
	mouseY = (y - wy) / height;
}
*/
static int error(const char *s) {
    printf("%s\n", s);
    return -1;
}
