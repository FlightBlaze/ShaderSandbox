cmake_minimum_required(VERSION 3.7)
project(shadersandbox)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

pkg_check_modules(glfw3 REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(wayland-client REQUIRED wayland-client)
pkg_check_modules(wayland-server REQUIRED wayland-server)

file(GLOB SRC "*.cc")
file(GLOB H "*.h")

file(GLOB STB_IMAGE_H "third_party/stb_image/stb_image.h")
file(GLOB STB_IMAGE_C "third_party/stb_image/stb_image.c")

add_executable(shadersandbox ${SRC} ${H} ${STB_IMAGE_H} ${STB_IMAGE_C} main.cc)

target_include_directories(shadersandbox PRIVATE .)

target_link_libraries(shadersandbox
    Threads::Threads
    OpenGL::OpenGL
    GLEW::GLEW
    EGL
    glfw
	wayland-server
	wayland-client
    ${GLM_LIBRARIES}
)

target_include_directories(shadersandbox PUBLIC ${GLM_INCLUDE_DIRS})
target_compile_options(shadersandbox PUBLIC ${GLM_CFLAGS_OTHER})

install(TARGETS shadersandbox RUNTIME DESTINATION bin)
