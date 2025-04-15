#include "constraint.hpp"
#include "method.hpp"

namespace ConstraintGraph {

Constraint::Constraint(long long int strenght) : strength_(strenght) {};

void Constraint::addMethod(std::unique_ptr<Method>&& method) {
    methods_.push_back(std::move(method));
}

void Constraint::selectMethod(Method* method) {
    selected_method_ = method;
}

void Constraint::process() {
    selected_method_->process();
}

const Method* Constraint::getSelectedMethod() const {
	return selected_method_;
}

const std::vector<std::unique_ptr<Method>>& Constraint::getMethods() const {
    return methods_;
}

std::vector<std::unique_ptr<Method>>& Constraint::getMethods() {
    return methods_;
}

const std::int64_t Constraint::getStrength() const {
    return strength_;
}

const bool Constraint::isSatisfied() const { 
    return is_satisfy_;
}

void Constraint::setSatisfied(bool satisfy) {
    is_satisfy_ = satisfy;
}
    
} // namespace ConstraintGraph
