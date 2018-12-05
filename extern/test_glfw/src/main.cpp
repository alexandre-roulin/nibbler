//#define STB_IMAGE_IMPLEMENTATION
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
#include "Model.hpp"
#include "Mesh.hpp"

int main() {
    DisplayGlfw lol(NULL, 35, 32, 32, "Issou");

    lol.update();
    Glfw glfw("Coucou", 1024, 720);
    Shader shader;


    std::string file_path = __FILE__;
    std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
    dir_path = dir_path.substr(0, dir_path.rfind("\\"));
    std::cout << dir_path << std::endl;

    shader.attach(dir_path + "\\shader\\basic.frag");
    std::cout << "Frag passed" << std::endl;
    shader.attach(dir_path + "\\shader\\basic.vert");
    std::cout << "Vert passed" << std::endl;
    shader.link();
    std::cout << "<Linked passed" << std::endl;

    //Add BOth at Construction :!!!!!

    float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Model model;

    std::vector<Vertex> vertex;
    vertex.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
    		));
	vertex.emplace_back(Vertex(
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec2(0.5f, -0.5f)
	));
	vertex.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec2(-0.5f, -0.5f)
	));
	vertex.emplace_back(Vertex(
			glm::vec3(-0.5f,  0.5f, 0.0f),
			glm::vec3(-0.5f,  0.5f, 0.0f),
			glm::vec2(-0.5f,  0.5f)
	));

    std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};

    Mesh mesh(vertex, indices);


    // Rendering Loop
    shader.activate();
    while (!glfw.exit()) {
        glfw.update();
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 transform;
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        /*glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
		*/
        mesh.render();
        // Flip Buffers and Draw
        glfw.render();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return EXIT_SUCCESS;

}