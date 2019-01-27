#include "ActModel.hpp"

ActModel::ActModel() :
		model_(nullptr),
		flag_(0),
		transform_(glm::mat4(1.f)),
		rotate_(glm::mat4(1.f)),
		sameScaling_(1.f),
		scaling_(1.f),
		rotation_(0.f),
		position_(0.f),
		speed_(1.f) {
}

ActModel::ActModel(Model const *model) :
		model_(model),
		flag_(0),
		transform_(glm::mat4(1.f)),
		rotate_(glm::mat4(1.f)),
		sameScaling_(1.f),
		scaling_(1.f),
		rotation_(0.f),
		position_(0.f),
		speed_(1.f) {
}

ActModel::~ActModel() {
	clean_();
}

void ActModel::clean_() {
}

ActModel::ActModel(ActModel const &src) {
	*this = src;
}
ActModel &ActModel::operator=(ActModel const &src) {
	if (this != &src) {
		model_ = src.model_;
		flag_ = src.flag_;
		transform_ = src.transform_;
		rotate_ = src.rotate_;
		sameScaling_ = src.sameScaling_;
		scaling_ = src.scaling_;
		rotation_ = src.rotation_;
		position_ = src.position_;
		speed_ = src.speed_;
	}
	return (*this);
}

void ActModel::assign(Model const *model) {
	model_ = model;
}

void	ActModel::translate(const glm::vec3 &axis, float deltaTime) {
	float velocity = speed_ * deltaTime;

	position_ += (axis * velocity);
}
void	ActModel::rotate(glm::vec3 const &axis, float angle, float deltaTime) {
	float velocity = speed_ * deltaTime;

	rotate_ = glm::rotate(rotate_, (angle * velocity), axis);
}
void	ActModel::scale(glm::vec3 const &axis, float deltaTime) {
	float velocity = speed_ * deltaTime;

	scaling_ += (axis * velocity);
}
void	ActModel::scale(float scale, float deltaTime) {
	float velocity = speed_ * deltaTime;

	sameScaling_ += (scale * velocity);
}

void	ActModel::resetTransform() {
	transform_ = glm::mat4(1.0f);
	rotate_ = glm::mat4(1.0f);
	scaling_ = glm::vec3(1.f);
	sameScaling_ = 1.f;
	position_ = glm::vec3(0.f);
}

void	ActModel::render(Shader &shader, GLenum typeOfDraw) {
	updateTransform_();
	glm::mat4 modelMatrix = transform_;
	shader.setMat4("model", modelMatrix);
	model_->render(shader, typeOfDraw);
}

void	ActModel::updateTransform_() {
	glm::mat4 scale(1.f);
	glm::mat4 toOrigin(1.f);

	if (model_) {
		if (flag_.test(ActModel::eFlag::SAME_SCALING))
			scale = glm::scale(scale, glm::vec3(model_->getInterScaling() * sameScaling_));
		else
			scale = glm::scale(scale, (model_->getInterScaling() * scaling_));
		toOrigin = glm::translate(glm::mat4(1.f), -model_->getPositionCenterRelativeToOrigin());
	}
	transform_ = glm::translate(glm::mat4(1.f), position_)
				 //* glm::translate(glm::mat4(1.f), model_->getPositionCenterRelativeToOrigin())
				* rotate_
				* scale
				* toOrigin;

}

Model const		*ActModel::getModel() const {
	return (model_);
}

glm::mat4	ActModel::getTransform() const {
	return (transform_);
}

glm::vec3	ActModel::getPosition() const {
	return (position_);
}

bool	ActModel::debug_ = true;

