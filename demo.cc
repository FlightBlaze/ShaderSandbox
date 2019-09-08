#include "demo.h"

// Important: include GLEW before other graphic libraries
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <GL/gl.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "shader.h"

#include "third_party/stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static
void key_callback(GLFWwindow*, int, int, int, int);
void error_callback(int, const char*);
static
int error(const char*);

namespace sandbox {

GLfloat vertices[] = {
     // Position          // Color            // UVs
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
     1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top left
};

const GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
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

Demo :: Demo(int argc, char **argv):
    m_argc(), m_argv() {
    
    
}

Demo :: ~Demo() {
}

int Demo :: run() {
    // Init
    
    if (!glfwInit())
        return error("Failed to initialize GLFW3");
    
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    m_window = glfwCreateWindow(920, 767, "Demo", 0, 0);
    
    glfwSetKeyCallback(m_window, key_callback);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    
    // Init GLEW
    
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
        return error("Failed to initialize GLEW");
    
    Shader sampleShader(
        "../ShaderSandbox/shaders/sample.vert",
        "../ShaderSandbox/shaders/sample.frag"
    );
    
    // Create buffers and array
    
    GLuint vertexBufferObject, vertexArrayObject, elementBufferObject;
    
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &elementBufferObject);
    
    // Copy vertices
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    
    // Copy indices
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set pointers to vertex attributes
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Position
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat))); // Color
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // UVs
    glEnableVertexAttribArray(2);
    
    // Unbind buffers and array
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Make textures
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GLuint texture0 = loadImage("../ShaderSandbox/resources/sample.png");
    GLuint texture1 = loadImage("../ShaderSandbox/resources/photo1.png");
    
    // Main cycle
    
    do {
        int width, height;
        
        glfwGetFramebufferSize(m_window, &width, &height);
        glViewport(0, 0, width, height);
    
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Bind textures
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUniform1i(glGetUniformLocation(sampleShader.program, "ourTexture0"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(sampleShader.program, "ourTexture1"), 1);
        
        // Push width and height
        
        glUniform1i(glGetUniformLocation(sampleShader.program, "height"), height);
        glUniform1i(glGetUniformLocation(sampleShader.program, "width"), width);
        
        // Animate
        
        GLfloat anim = (sin(glfwGetTime()) / 2) + 0.5;
        glUniform1f(glGetUniformLocation(sampleShader.program, "anim"), anim);
        
        // Transform
        
        //glm::mat4 trans;
        // ...
        
        //GLuint transformLoc = glGetUniformLocation(sampleShader.program, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        // Render
        
        sampleShader.use();
        
        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    while (!glfwWindowShouldClose(m_window));
    
    // Ternimate
    
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteBuffers(1, &vertexBufferObject);
    
    glfwDestroyWindow(m_window);
    glfwTerminate();
    
    return 0;
}

    
} // end namespace sandbox

void error_callback(int error, const char* description)
 {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static int error(const char *s) {
    printf("%s\n", s);
    return -1;
}

