#include "variable.hpp"
#include "method.hpp"

#include <algorithm>
#include <cstdint>


namespace ConstraintGraph {


Variable::Variable(std::any value) : value(std::move(value)) {}

const std::vector<Method*>& Variable::getOutputsMethods() const { return outputs; }

std::vector<Method*> Variable::getOutputsMethods() { return outputs; }

const std::vector<Method*>& Variable::getInputsMethods() const { return inputs; }

std::vector<Method*> Variable::getInputsMethods() { return inputs; }

void Variable::addInput(Method* method) {
    inputs.push_back(std::move(method));
}

void Variable::addOutput(Method* method) {
    outputs.push_back(std::move(method));
}

void Variable::removeInput(Method* method) {
    inputs.erase(
        std::remove(inputs.begin(), inputs.end(), method),
        inputs.end()
    );
}


void Variable::removeOutput(Method* method) {
    outputs.erase(
        std::remove(outputs.begin(), outputs.end(), method),
        outputs.end()
    );
}

void Variable::setDefiningMethod(Method* method) {
    defining_method = method;
}

const int64_t Variable::getForce() const {
    return force;
}

void Variable::setForce(std::int64_t new_force) {
    force = new_force;
}

Method* Variable::getDefiningMethod() {
    return defining_method;
}

Constraint* Variable::getDefiningConstraint() {
    return defining_method->parent;
}

    
} // namespace ConstraintGraph
