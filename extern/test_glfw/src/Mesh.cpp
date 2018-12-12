
#include "Mesh.hpp"
#include "Shader.hpp"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertice, std::vector<unsigned int> indice, std::vector<Texture> texture) :
	flag_(Mesh::eFlag::TEXTURE),
	vertice_(vertice),
	indice_(indice),
	texture_(texture) {
	setupMesh_();
}

Mesh::Mesh(std::vector<Vertex> vertice, std::vector<unsigned int> indice) :
		flag_(Mesh::eFlag::NONE),
		vertice_(vertice),
		indice_(indice) {
	setupMesh_();
}

Mesh::~Mesh() {
	clean_();
}

void    Mesh::clean_() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

unsigned int		Mesh::getVAO() const {
	return (VAO);
}

void	Mesh::render(Shader &shader) const {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;

	for(unsigned int i = 0; i < texture_.size(); i++) {

		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name;
		if (texture_[i].type == Texture::eType::SPECULAR) {
			name = "texture_specular";
		}
		if (texture_[i].type == Texture::eType::DIFFUSE) {
			name = "texture_diffuse";
		}

		if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);
		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, texture_[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	render();
}

void	Mesh::render() const noexcept {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indice_.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void	Mesh::setupMesh_() {


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_.size() * sizeof(Vertex), &vertice_[0], GL_STATIC_DRAW);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_.size() * sizeof(unsigned int), &indice_[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);


	glDeleteBuffers(1, & VBO);
	glDeleteBuffers(1, & EBO);
}

bool        Mesh::debug_ = true;
