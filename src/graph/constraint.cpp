#include "constraint.hpp"

#include <cassert>

#include "method.hpp"
#include "variable.hpp"

namespace NSConstraintGraph {

Constraint::Constraint(const std::vector<Variable*>& variables, int priority)
: variables_{variables}, priority_{priority} {};

void Constraint::addMethod(std::unique_ptr<Method>&& method) {
    methods_.push_back(std::move(method));
}

const std::vector<std::unique_ptr<Method>>& Constraint::getMethods() const {
    return methods_;
}

const std::vector<Variable*>& Constraint::getVariables() const {
    return variables_;
}

std::vector<Variable*> Constraint::getVariables() {
    return variables_;
}

void Constraint::selectMethod(Method* method) {
    selected_method_ = method;
}

void Constraint::processSelectedMethod() {
    selected_method_->process();
}

const Method* Constraint::getSelectedMethod() const {
    return selected_method_;
}

int Constraint::getPriority() const {
    return priority_;
}

bool Constraint::isSatisfied() const {
    return is_satisfied_;
}

bool Constraint::isStay() const {
    return is_stay_;
}

bool Constraint::isEnabled() const {
    return is_enabled_;
}

bool Constraint::isBlocked() const {
    for (auto variable : variables_) {
        if (variable->getForce() > priority_){
            return true;
        }
    }
    return false;
}

void Constraint::enable() {
    is_enabled_ = true;
}

void Constraint::disable() {
    is_satisfied_ = false;
    is_enabled_ = false;
}

void Constraint::satisfyByMethodIndex(int index) {
    assert(!is_satisfied_);
    //is_satisfied_ = true;
    selected_method_ = methods_.at(index).get();
    selected_method_->satisfy();
}

void Constraint::satisfy(Method* method) {
    assert(method);
    //is_satisfied_ = true;
    selected_method_ = method;
    selected_method_->satisfy();
}

void Constraint::unsatisfy() {
    //is_satisfied_ = false;
    assert(selected_method_);
    selected_method_->unsatisfy();
    //selected_method_ = nullptr;
}

Method* Constraint::findMaxPriorMethod() {
    Method* max_method = methods_.front().get();
    assert(max_method->getOutput());

    for (auto& current_method : methods_) {
        if (current_method->getOutput()->getForce() > max_method->getOutput()->getForce()) {
            max_method = current_method.get();
            assert(max_method->getOutput());
        }

    }
    return max_method;
}

}  // namespace NSConstraintGraph
