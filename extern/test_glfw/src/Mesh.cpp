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
}

Mesh::Mesh(Mesh const &mesh) {
	*this = mesh;
}
Mesh &Mesh::operator=(Mesh const &mesh) {
	if (this != &mesh) {
		vertice_ = mesh.vertice_;
		indice_ = mesh.indice_;
		texture_ = mesh.texture_;
	}

	return (*this);
}

void	Mesh::render(Shader &shader) {
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
		if (texture_[i].type == Texture::eType::NORMAL) {
			name = "texture_normal";
		}

		if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);
		else if(name == "texture_normal")
			number = std::to_string(normalNr++);
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
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_.size() * sizeof(Vertex), &vertice_[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_.size() * sizeof(unsigned int), &indice_[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	glBindVertexArray(0);
}

bool        Mesh::debug_ = true;
