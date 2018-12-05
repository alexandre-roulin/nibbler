#include "Mesh.hpp"
#include "Shader.hpp"

Mesh::Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indice, std::vector<Texture> texture) :
	flag_(Mesh::eFlag::TEXTURE),
	vertex_(vertex),
	indice_(indice),
	texture_(texture) {
	setupMesh_();
}

Mesh::Mesh(std::vector<Vertex> vertex, std::vector<unsigned int> indice) :
		flag_(Mesh::eFlag::NONE),
		vertex_(vertex),
		indice_(indice) {
	setupMesh_();
}

Mesh::~Mesh() {
	clean_();
}

void    Mesh::clean_() {
}

void	Mesh::render(Shader &shader) {
	/*-
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for(unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i); // Activation de l’unité de texture adéquate avant liaison
		// récupère le numéro de la texture (le N dans diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++);
		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	*/
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
	glBufferData(GL_ARRAY_BUFFER, vertex_.size() * sizeof(Vertex), &vertex_[0], GL_STATIC_DRAW);
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
