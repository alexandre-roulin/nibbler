#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "DisplayGlfw.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

#include "Glfw.hpp"

int main() {
    DisplayGlfw lol(NULL, 35, 32, 32, "Issou");

    Shader shader;

    lol.update();
    Glfw glfw("Coucou", 1024, 720);
    // Rendering Loop
    while (!glfw.exit()) {
        glfw.update();
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transform;
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // Flip Buffers and Draw
        glfw.render();
    }
    return EXIT_SUCCESS;

}