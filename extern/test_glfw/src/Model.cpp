#include "Model.hpp"
#include "Mesh.hpp"

Model::Model(std::string const &path) :
    path_(path){
    loadModel_();
}

Model::~Model() {
    clean_();
}

void    Model::clean_() {
}

void	Model::render() const {
    for(unsigned int i = 0; i < meshe_.size(); i++)
        meshe_[i].render();
}
void	Model::render(Shader &shader) {
    for(unsigned int i = 0; i < meshe_.size(); i++)
        meshe_[i].render(shader);
}

void 					Model::loadModel_() {
}
/*
void					Model::processNode_(aiNode *node, const aiScene *scene) {
}
Mesh					Model::processMesh_(aiMesh *mesh, const aiScene *scene) {
}*/

bool        Model::debug_ = true;