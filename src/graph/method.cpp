#include "method.hpp"

#include "constraint.hpp"
#include "variable.hpp"

namespace ConstraintGraph {

Method::Method(std::function<void()> func, std::vector<Variable*> inputs, Variable* output, Constraint* parent)
        : function(std::move(func)), 
          inputs(std::move(inputs)), 
          output(std::move(output)), 
          parent(std::move(parent)) {};


void Method::process() {
    function();
}

Constraint* Method::getConstrain() { 
    return parent;
}

const Constraint* Method::getConstrain() const { 
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

const  std::vector<Variable*> Method::getInputs() const { 
    return inputs;
}

bool Method::isSelected() {
    return this == parent->getSelectedMethod();
}

void Method::satisfy() {
    output->setDefiningMethod(this);

	for (auto variable : inputs) {
		variable->addOutput(this);
	}
}
void Method::unsatisfy() {
    output->setDefiningMethod(nullptr);

	for (Variable* variable : inputs) {
		variable->removeOutput(this);
	}
}

    
} // namespace ConstraintGraph
