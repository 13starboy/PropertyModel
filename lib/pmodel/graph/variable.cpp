#include "variable.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>

#include <pmodel/graph/method.hpp>

namespace NSPropertyModel {

Variable::Variable(std::any value) : value_(std::move(value)) {}

const std::vector<Method*>& Variable::getDependentMethods() const {
    return dependent_methods_;
}

std::vector<Method*> Variable::getDependentMethods() {
    return dependent_methods_;
}

void Variable::addDependentMethod(Method* method) {
    dependent_methods_.push_back(std::move(method));
}

void Variable::removeDependentMethod(Method* method) {
    auto iter_to_delete = std::find(dependent_methods_.begin(), dependent_methods_.end(), method);
    if (iter_to_delete != dependent_methods_.end()) {
        dependent_methods_.erase(iter_to_delete);
    }

    // dependent_methods_.erase(
    //     std::remove(dependent_methods_.begin(), dependent_methods_.end(), method), dependent_methods_.end()
    // );
}

void Variable::setDefiningMethod(Method* method) {
    defining_method_ = method;
}

const int Variable::getForce() const {
    return force_;
}

void Variable::setForce(int new_force) {
    force_ = new_force;
}

void Variable::setValue(std::any value) {
    value_ = value;
}

std::any Variable::getValue() const {
    return value_;
}

Method* Variable::getDefiningMethod() {
    return defining_method_;
}

Constraint* Variable::getDefiningConstraint() {
    assert(defining_method_);
    return defining_method_->parent;
}

}  // namespace NSPropertyModel
