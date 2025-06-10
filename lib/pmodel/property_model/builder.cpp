#include "builder.hpp"

#include <iostream>

namespace NSPropertyModel {

Builder::Builder() : propertyModel(std::make_unique<PropertyModel>()) {}

void Builder::addVariable(PropertyModel::Type type, const std::string& name, std::any value) {
    propertyModel->addVariable(type, name, value);
}

void Builder::addConstraint(const std::vector<std::string>& variables, int priority, bool enabled) {
    if (currentConstraint) {
        propertyModel->addConstraint(std::move(currentConstraint));
    }

    auto refs = propertyModel->bindVariables(variables);
    currentConstraint = std::make_unique<Constraint>(refs, priority);
    if (enabled) {
        currentConstraint->enable();
    } else {
        currentConstraint->disable();
    }
}

void Builder::addMethod(
    std::function<std::any(const std::vector<std::any>&)> function, const std::vector<std::string>& inputs,
    const std::string& output
) {
    auto method = propertyModel->bindMethod(function, inputs, output);
    auto input_refs = propertyModel->bindVariables(inputs);
    auto output_refs = propertyModel->bindVariables({output});

    currentConstraint->addMethod(std::make_unique<Method>(method, input_refs, output_refs[0], currentConstraint.get()));
}

std::unique_ptr<PropertyModel> Builder::extract() {
    if (currentConstraint) {
        propertyModel->addConstraint(std::move(currentConstraint));
    }

    propertyModel->addStayConstraints();
    propertyModel->prepareSolutionGraph();
    return std::move(propertyModel);
}
}  // namespace NSPropertyModel
