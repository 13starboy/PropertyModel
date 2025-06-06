#include "method.hpp"

#include <pmodel/graph/constraint.hpp>
#include <pmodel/graph/variable.hpp>

namespace NSPropertyModel {

Method::Method(std::function<void()> func, std::vector<Variable*> inputs, Variable* output, Constraint* parent)
    : function(std::move(func)), inputs(std::move(inputs)), output(std::move(output)), parent(std::move(parent)) {};

void Method::process() const {
    function();
}

Constraint* Method::getConstraint() {
    return parent;
}

const Constraint* Method::getConstraint() const {
    return parent;
}

Variable* Method::getOutput() {
    return output;
}

const Variable* Method::getOutput() const {
    return output;
}

std::vector<Variable*> Method::getInputs() {
    return inputs;
}

const std::vector<Variable*> Method::getInputs() const {
    return inputs;
}

bool Method::isSelected() {
    return this == parent->getSelectedMethod();
}

void Method::satisfy() {
    output->setDefiningMethod(this);

    for (auto variable_ptr : inputs) {
        variable_ptr->addDependentMethod(this);
    }
}

void Method::unsatisfy() {
    output->setDefiningMethod(nullptr);

    for (auto variable_ptr : inputs) {
        variable_ptr->removeDependentMethod(this);
    }
}

}  // namespace NSPropertyModel
