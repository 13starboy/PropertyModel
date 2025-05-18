#include "constraint_graph.hpp"

namespace ConstraintGraph {

void ConstraintGraph::AddConstraint(Constraint&& constraint) {
    constraints_.push_back(std::make_unique<Constraint>(std::move(constraint)));
}

void ConstraintGraph::AddConstraint(std::unique_ptr<Constraint> constraint) {
    constraints_.push_back(std::move(constraint));
}

void ConstraintGraph::AddVariable(Variable&& variable) {
    variables_.push_back(std::make_unique<Variable>(std::move(variable)));
}

void ConstraintGraph::AddVariable(std::unique_ptr<Variable> variable) {
    variables_.push_back(std::move(variable));
}

std::vector<std::unique_ptr<Constraint>>& ConstraintGraph::getConstraints() {
    return constraints_;
}

const std::vector<std::unique_ptr<Constraint>>& ConstraintGraph::getConstraints() const {
    return constraints_;
}

std::vector<std::unique_ptr<Variable>>& ConstraintGraph::getVariables() {
    return variables_;
}

const std::vector<std::unique_ptr<Variable>>& ConstraintGraph::getVariables() const {
    return variables_;
}

Constraint* ConstraintGraph::getConstraintByIndex(size_t i)
{
    return constraints_.at(i).get();
}

    
} // namespace ConstraintGraph
