#include "constraint.hpp"
#include "method.hpp"
#include "variable.hpp"

namespace ConstraintGraph {

Constraint::Constraint(int64_t priority) : priority_(priority) {};

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

const std::int64_t Constraint::getPriority() const {
    return priority_;
}

const bool Constraint::isSatisfied() const { 
    return is_satisfy_;
}

void Constraint::setSatisfied(bool satisfy) {
    is_satisfy_ = satisfy;
}

bool Constraint::isBlocked() {
    bool is_blocked = false;
	for (const auto& method : methods_) {
        Variable* output = method.get()->getOutput();
		if (output->getForce() > priority_) {
			is_blocked = true;
		}
	}
    return is_blocked;
}

void Constraint::enable() {
    state = State::Enabled;
}

void Constraint::satisfy(Method* method) {
    is_satisfy_ = true;
    selected_method_ = method;
    selected_method_->satisfy();
}

void Constraint::unsatisfy() {
    is_satisfy_ = false;
    selected_method_->unsatisfy();
    selected_method_ = nullptr;
}

Method* Constraint::findMaxPriorMethod() {
    Method* max_method = methods_[0].get();
    for (auto& current_method : methods_)
    {
        if (current_method->getOutput()->getForce() > max_method->getOutput()->getForce()) {
            max_method = current_method.get();
        }
    }
    return max_method;
}

    
} // namespace ConstraintGraph
