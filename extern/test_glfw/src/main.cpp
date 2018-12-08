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
#include <fstream>

int main(int argc, char **argv) {
    DisplayGlfw lol(NULL, 35, 32, 32, "Issou");


    lol.update();
    Glfw glfw("Coucou", 1024, 720);


	glEnable(GL_DEPTH_TEST);
	glPointSize(5.0);
	glDepthFunc(GL_LESS);

    Shader shader;


	std::string file_path = __FILE__;

#ifdef __APPLE__

	std::string dir_path = file_path.substr(0, file_path.rfind("/"));
    dir_path = dir_path.substr(0, dir_path.rfind("/"));
    std::cout << dir_path << std::endl;

	std::ifstream t(dir_path + "/file.txt");
	std::string path_model((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());


    shader.attach(dir_path + "/shader/basic.frag");
    std::cout << "Frag passed" << std::endl;
    shader.attach(dir_path + "/shader/basic.vert");
    std::cout << "Vert passed" << std::endl;

#else
	std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
	dir_path = dir_path.substr(0, dir_path.rfind("\\"));
	std::cout << dir_path << std::endl;

	std::ifstream t(dir_path + "\\file.txt");
	std::string path_model((std::istreambuf_iterator<char>(t)),
						   std::istreambuf_iterator<char>());


	shader.attach(dir_path + "\\shader\\basic.frag");
	std::cout << "Frag passed" << std::endl;
	shader.attach(dir_path + "\\shader\\basic.vert");
	std::cout << "Vert passed" << std::endl;
#endif

	shader.link();
	std::cout << "<Linked passed" << std::endl;

	std::cout << path_model << std::endl;

	Model model(path_model);
	std::cout << "Model : [" << model.path_ << "] : [" << model.directory_ << "]" << std::endl;
	std::cout << "Nombre de mesh : [" << model.mesh_.size() << "]" << std::endl;
	for (unsigned int i = 0; i < model.mesh_.size(); i++) {
		std::cout << "Mesh {" << i << " / " << model.mesh_.size() << "}" << std::endl;
		std::cout << "Incides [" << model.mesh_[i].indice_.size() << "]" << std::endl;
		std::cout << "Vertice [" << model.mesh_[i].vertice_.size() << "]" << std::endl;
		std::cout << "Texture [" << model.mesh_[i].texture_.size() << "]" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;

		//for (unsigned int j = 0; j < model.mesh_[i].indice_.size(); j++) {
		//	std::cout << "J[" <<  j  << "] "<< model.mesh_[i].indice_[j] << " | " << model.mesh_[i].vertice_[j].position.x << "," << model.mesh_[i].vertice_[j].position.y << "," << model.mesh_[i].vertice_[j].position.z << std::endl;
		//	std::cout << "J[" <<  j  << "] "<< model.mesh_[i].indice_[j] << " | " << model.mesh_[i].vertice_[j].normal.x << "," << model.mesh_[i].vertice_[j].normal.y << "," << model.mesh_[i].vertice_[j].normal.z << std::endl;
		//	std::cout << "J[" <<  j  << "] "<< model.mesh_[i].indice_[j] << " | " << model.mesh_[i].vertice_[j].uv.x << "," << model.mesh_[i].vertice_[j].uv.y << std::endl;
		//}

	}
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;

//	Model alol("C:\\Users\\ntoniolo\\CLionProjects\\nibbler\\extern\\test_glfw\\resources\\objects\\nanosuit\\nanosuit.obj");
/*
	std::cout << "Nombre de mesh : [" << alol.mSubMeshes.size() << "]" << std::endl;
	for (unsigned int i = 0; i < alol.mSubMeshes.size(); i++) {
		std::cout << "Mesh {" << i << " / " << alol.mSubMeshes.size() << "}" << std::endl;
		std::cout << "Incides [" << alol.mSubMeshes[i]->mIndices.size() << "]" << std::endl;
		std::cout << "Vertice [" << alol.mSubMeshes[i]->mVertices.size() << "]" << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;

		//for (unsigned int j = 0; j < model.mesh_[i].indice_.size(); j++) {
		//	std::cout << "J[" <<  j  << "] "<< model.mesh_[i].indice_[j] << " | " << model.mesh_[i].vertice_[j].position.x << "," << model.mesh_[i].vertice_[j].position.y << "," << model.mesh_[i].vertice_[j].position.z << std::endl;
		//	std::cout << "J[" <<  j  << "] "<< model.mesh_[i].indice_[j] << " | " << model.mesh_[i].vertice_[j].normal.x << "," << model.mesh_[i].vertice_[j].normal.y << "," << model.mesh_[i].vertice_[j].normal.z << std::endl;
		//	std::cout << "J[" <<  j  << "] "<< model.mesh_[i].indice_[j] << " | " << model.mesh_[i].vertice_[j].uv.x << "," << model.mesh_[i].vertice_[j].uv.y << std::endl;
		//}

	}
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
	std::cout << "---------------------------------------------------------------" << std::endl;
 */



glm::mat4 projection = glm::mat4(1.0);
projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)720, 0.1f, 100000.0f);
glm::mat4 view = glm::mat4(1.0);
view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.0f));
glm::mat4 m_model = glm::mat4(1.0);
m_model = glm::translate(m_model, glm::vec3(0.0f, -10.0f, -30.0f));


    while (!glfw.exit()) {

        glfw.update();
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
		if (glfwGetKey(glfw.getWindow(), GLFW_KEY_G) == GLFW_PRESS)
			m_model = glm::rotate(m_model, glm::radians(-1.0f), glm::vec3(0.f, -1.0f, -0.0f));
		/*
		 * for(int i = 0; i < 4; i++) {
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

		GLint total_mem_kb = 0;
		glGetIntegerv(0x9048,
					  &total_mem_kb);

		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(0x9049,
					  &cur_avail_mem_kb);

		//std::cout << "total_mem_kb : " << total_mem_kb << std::endl;
		//std::cout << "cur_avail_mem_kb : " << cur_avail_mem_kb << std::endl;

		//model.mesh_[1].render(shader);
		//model.mesh_[2].render(shader);

		model.render(shader);

        glfw.render();
    }

    return EXIT_SUCCESS;

}