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
	updateTransform_();
}

ActModel::~ActModel() {
	clean_();
}

void ActModel::clean_() {
}

void ActModel::assign(Model const *model) {
	model_ = model;
}

void	ActModel::translate(const glm::vec3 &axis, float deltaTime) {
	float velocity = speed_ * deltaTime;

	position_ += (axis * velocity);
	updateTransform_();
}
void	ActModel::rotate(glm::vec3 const &axis, float angle, float deltaTime) {
	float velocity = speed_ * deltaTime;

	rotate_ = glm::rotate(rotate_, (angle * velocity), axis);
	updateTransform_();
}
void	ActModel::scale(glm::vec3 const &axis, float deltaTime) {
	float velocity = speed_ * deltaTime;

	scaling_ += (axis * velocity);
	updateTransform_();
}
void	ActModel::scale(float scale, float deltaTime) {
	float velocity = speed_ * deltaTime;

	sameScaling_ += (scale * velocity);
	updateTransform_();
}

void	ActModel::resetTransform() {
	transform_ = glm::mat4(1.0f);
	rotate_ = glm::mat4(1.0f);
	scaling_ = glm::vec3(1.f);
	sameScaling_ = 1.f;
	position_ = glm::vec3(0.f);
	updateTransform_();
}

void	ActModel::updateTransform_() {
	glm::mat4 scale(1.f);

	if (flag_.test(ActModel::eFlag::SAME_SCALING))
		scale = glm::scale(scale, glm::vec3(model_->getInterScaling() * sameScaling_));
	else
		scale = glm::scale(scale, (model_->getInterScaling() * scaling_));
	transform_ = glm::translate(glm::mat4(1.f), position_)
				 * scale
				 * glm::translate(glm::mat4(1.f), model_->getPositionCenter())
				 * rotate_
				 * glm::translate(glm::mat4(1.f), -model_->getPositionCenter());
}

Model const		*ActModel::getModel() const {
	return (model_);
}

glm::mat4	ActModel::getTransform() const {
	return (transform_);
}

bool	ActModel::debug_ = true;

