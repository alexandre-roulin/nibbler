#include "ActModel.hpp"

ActModel::ActModel() :
		Transform(),
		flag_(0),
		model_(nullptr) {}

ActModel::ActModel(Model const *model) :
		Transform(),
		flag_(0),
		model_(model) {
	setCenter(model->getPositionCenterRelativeToOrigin());
	setInterScaling(model->getInterScaling());
}

ActModel::ActModel(ActModel const &src) :
		Transform(src),
		flag_(src.flag_),
		model_(src.model_) {}

void ActModel::assign(Model const *model) {
	model_ = model;
	setCenter(model->getPositionCenterRelativeToOrigin());
	setInterScaling(model->getInterScaling());
}

void	ActModel::render(Shader &shader, GLenum typeOfDraw) {
	updateTransform();
	glm::mat4 modelMatrix = transform_;
	shader.setMat4("model", modelMatrix);
	model_->render(shader, typeOfDraw);
}

Model const		*ActModel::getModel() const {
	return (model_);
}
