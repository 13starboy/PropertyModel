#include "constraint_graph.hpp"

namespace ConstraintGraph {

void ConstraintGraph::addConstraint(Constraint&& constraint) {
    constraints_.push_back(std::make_unique<Constraint>(std::move(constraint)));
}

void ConstraintGraph::addConstraint(std::unique_ptr<Constraint> constraint) {
    constraints_.push_back(std::move(constraint));
}

void ConstraintGraph::addVariable(Variable&& variable) {
    variables_.push_back(std::make_unique<Variable>(std::move(variable)));
}

void ConstraintGraph::addVariable(std::unique_ptr<Variable> variable) {
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

std::unordered_map<Variable*, Constraint*>& ConstraintGraph::getStayEdges() {
    return stay_edges_;

}
const std::unordered_map<Variable*, Constraint*>& ConstraintGraph::getStayEdges() const {
    return stay_edges_;
}

Constraint* ConstraintGraph::getConstraintByIndex(size_t i)
{
    return constraints_.at(i).get();
}

void ConstraintGraph::markStayDefined(Variable* variable) {
    auto constraint = stay_edges_[variable];
    constraint->satisfy(constraint->getMethods()[0].get());
}

    
} // namespace ConstraintGraph
