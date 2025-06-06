#include "property_model.hpp"

#include <iostream>

namespace NSPropertyModel {
void PropertyModel::set(const std::string& name, std::any value) {
    Variable* variable = variables_[name];

    DeltaBlueSolver::disableConstraintByIndex(constraint_graph_, constraint_graph_.findStayIndex(variable));
    try {
        variable->setValue(value);
    } catch (const std::bad_any_cast& e) {
        throw std::runtime_error("Type mismatch in variable " + name + ": " + e.what());
    }
    constraint_graph_.processMethods();
}

std::any PropertyModel::get(const std::string& name) {
    Variable* variable = variables_[name];
    return variable->getValue();
}

void PropertyModel::enableConstraint(size_t index) {
    DeltaBlueSolver::enableConstraintByIndex(constraint_graph_, index);
    constraint_graph_.processMethods();
}

void PropertyModel::disableConstraint(size_t index) {
    DeltaBlueSolver::disableConstraintByIndex(constraint_graph_, index);
    constraint_graph_.processMethods();
}

void PropertyModel::addVariable(Type belong, const std::string& name, std::any value) {
    auto variable_uptr = std::make_unique<Variable>(value);
    variable_uptr->name = name;
    variables_.insert({name, variable_uptr.get()});
    constraint_graph_.addVariable(std::move(variable_uptr));

    switch (belong) {
        case Type::DataVariable:
            data_.push_back(name);
            break;
        case Type::ValueVariable:
            value_.push_back(name);
            break;
        case Type::OutputVariable:
            output_.push_back(name);
            break;
    }
}

std::vector<Variable*> PropertyModel::bindVariables(const std::vector<std::string>& names) {
    std::vector<Variable*> references;
    references.reserve(names.size());
    for (const std::string& name : names) {
        references.push_back(variables_[name]);
    }
    return references;
}

std::function<void()> PropertyModel::bindMethod(
    std::function<std::any(const std::vector<std::any>&)> function, const std::vector<std::string>& inputs,
    const std::string& output
) {
    return [this, function = std::move(function), inputs = std::move(inputs), output = std::move(output)]() {
        std::vector<std::any> args;
        for (const auto& input : inputs) {
            args.push_back(variables_[input]->getValue());
        }
        std::any result = function(args);
        variables_[output]->setValue(result);
    };
}

void PropertyModel::addConstraint(std::unique_ptr<Constraint>&& constraint) {
    constraint_graph_.addConstraint(std::move(constraint));
}

void PropertyModel::addStayConstraints() {
    for (auto& [name, variable] : variables_) {
        constraint_graph_.addConstraint(Constraint::buildStayConstraint(variable, constraint_graph_.newStayPriority()));
        constraint_graph_.markStayDefined(variable);
    }
}

void PropertyModel::prepareSolutionGraph() {
    for (int constraint_index = 0; constraint_index < constraint_graph_.getConstraints().size(); ++constraint_index) {
        Constraint* current_constraint = constraint_graph_.getConstraintByIndex(constraint_index);
        if (current_constraint->isEnabled() && !current_constraint->isStay()) {
            DeltaBlueSolver::enableConstraintByIndex(constraint_graph_, constraint_index);
        }
    }
}
}  // namespace NSPropertyModel
