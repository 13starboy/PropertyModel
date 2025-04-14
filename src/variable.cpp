#include <method.hpp>
#include <variable.hpp>
#include <algorithm>


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

    
} // namespace ConstraintGraph
