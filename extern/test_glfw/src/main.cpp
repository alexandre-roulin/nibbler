#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "DisplayGlfw.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

#include "Glfw.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

int main(int argc, char **argv) {
    DisplayGlfw lol(NULL, 35, 32, 32, "Issou");


    lol.update();
    Glfw glfw("Coucou", 1024, 720);


	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	//glCullFace(GL_FRONT_AND_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader shader;


    std::string file_path = __FILE__;
    std::string dir_path = file_path.substr(0, file_path.rfind("/"));
    dir_path = dir_path.substr(0, dir_path.rfind("/"));
    std::cout << dir_path << std::endl;

    shader.attach(dir_path + "/shader/basic.frag");
    std::cout << "Frag passed" << std::endl;
    shader.attach(dir_path + "/shader/basic.vert");
    std::cout << "Vert passed" << std::endl;
    shader.link();
    std::cout << "<Linked passed" << std::endl;

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

	std::vector<Vertex> vertex2;


	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f,  0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));



	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));


	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f,  0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(-0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));

	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));
	vertex2.emplace_back(Vertex(
			glm::vec3(0.5f,  0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f, 0.0f),
			glm::vec2(0.5f,  0.5f)
	));


    std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};
	std::vector<unsigned int> indices2 = {0, 1, 2, 3, 4, 5, 6, 7, 8 ,9, 10, 11, 12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23};

    Mesh mesh(vertex, indices);

    Mesh cube(vertex2, indices2);


	std::vector<Vertex> vertex3;
	vertex3.emplace_back(Vertex(glm::vec3(1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(0.999999,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(0.999999,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(0.999999,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(0.999999,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(0.999999,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,-1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,-1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	vertex3.emplace_back(Vertex(glm::vec3(-1,1,1), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(0.5f, 0.5f)));
	std::vector<unsigned int> indices3 = {0, 1, 2, 3, 4, 5, 6, 7, 8 ,9, 10, 11, 12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,20 ,21 ,22 ,23, 24, 25 ,26 ,27, 28, 29 ,30 ,31 ,32 ,33 ,34 ,35};

	Mesh cubecube(vertex3, indices3);

	Model model(argv[1]);

	std::cout << "Model : [" << model.path_ << "] : [" << model.directory_ << "]" << std::endl;
	std::cout << "Nombre de mesh : [" << model.mesh_.size() << "]" << std::endl;
	for (unsigned int i = 0; i < model.mesh_.size(); i++) {
		std::cout << "Mesh {" << i << " / " << model.mesh_.size() << "}" << std::endl;
		std::cout << "Incides [" << model.mesh_[i]->indice_.size() << "]" << std::endl;
		std::cout << "Vertice [" << model.mesh_[i]->vertice_.size() << "]" << std::endl;
		std::cout << "Texture [" << model.mesh_[i]->texture_.size() << "]" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;


		for (unsigned int j = 0; j < model.mesh_[i]->indice_.size(); j++) {
			std::cout << "J[" <<  j  << "] "<< model.mesh_[i]->indice_[j] << " | " << model.mesh_[i]->vertice_[j].position.x << "," << model.mesh_[i]->vertice_[j].position.y << "," << model.mesh_[i]->vertice_[j].position.z << std::endl;
			std::cout << "J[" <<  j  << "] "<< model.mesh_[i]->indice_[j] << " | " << model.mesh_[i]->vertice_[j].normal.x << "," << model.mesh_[i]->vertice_[j].normal.y << "," << model.mesh_[i]->vertice_[j].normal.z << std::endl;
			std::cout << "J[" <<  j  << "] "<< model.mesh_[i]->indice_[j] << " | " << model.mesh_[i]->vertice_[j].uv.x << "," << model.mesh_[i]->vertice_[j].uv.y << std::endl;
		}

	}


	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;


	std::cout << "Incides [" << cubecube.indice_.size() << "]" << std::endl;
		std::cout << "Vertice [" << cubecube.vertice_.size() << "]" << std::endl;
		std::cout << "Texture [" << cubecube.texture_.size() << "]" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;


		for (unsigned int j = 0; j < cubecube.indice_.size(); j++) {
			std::cout << "J[" <<  j  << "] "<< cubecube.indice_[j] << " | " << cubecube.vertice_[j].position.x << "," << cubecube.vertice_[j].position.y << "," << cubecube.vertice_[j].position.z << std::endl;
			std::cout << "J[" <<  j  << "] "<< cubecube.indice_[j] << " | " << cubecube.vertice_[j].normal.x << "," << cubecube.vertice_[j].normal.y << "," << cubecube.vertice_[j].normal.z << std::endl;
			std::cout << "J[" <<  j  << "] "<< cubecube.indice_[j] << " | " << cubecube.vertice_[j].uv.x << "," << cubecube.vertice_[j].uv.y << std::endl;
		}


	glm::mat4 projection = glm::mat4(1.0);
	projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)720, 0.1f, 100000.0f);
	glm::mat4 view = glm::mat4(1.0);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.0f));
	glm::mat4 m_model = glm::mat4(1.0);
	m_model = glm::rotate(m_model, glm::radians(-55.0f), glm::vec3(0.0f, 0.0f, -3.0f));

	m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, -4.0f));


	Mesh abs = cubecube;

	//model.mesh_[0] = cubecube;

	// Rendering Loop
    while (!glfw.exit()) {

        glfw.update();
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		shader.activate();

		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
			m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, -1.0f));
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
			m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 1.0f));
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
			m_model = glm::translate(m_model, glm::vec3(1.0f, 0.0f, 0.0f));
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
			m_model = glm::translate(m_model, glm::vec3(-1.0f, 0.0f, 0.0f));
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
			m_model = glm::translate(m_model, glm::vec3(0.0f, 1.0f, 0.0f));
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_E) == GLFW_PRESS)
			m_model = glm::translate(m_model, glm::vec3(0.0f, -1.0f, 0.0f));
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
			glm::mat4 Lol(1.0);
			Lol = glm::rotate(Lol, glm::radians(-55.0f), glm::vec3(0.0f, 0.0f, -3.0f));
			m_model = glm::translate(Lol, glm::vec3(0.0f, 0.0f, -4.0f));
		}
		m_model = glm::rotate(m_model, glm::radians(-1.0f), glm::vec3(0.f, -0.0f, -3.0f));
		/*for(int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << view[i][j];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		 */
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", m_model);

        /*
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
		*/
        //mesh.render();

		model.render(shader);
		//model.mesh_[0].render(shader);
		//model.mesh_[0]->render();
		//mesh.render(shader);
		//mesh.render();
		//cubecube.render();
		//cubecube.render(shader);
		//abs.render();
        // Flip Buffers and Draw
        glfw.render();
    }

    return EXIT_SUCCESS;

}