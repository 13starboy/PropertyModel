#include "constraint_graph.hpp"

#include <cassert>
#include <iostream>
namespace NSConstraintGraph {

void ConstraintGraph::addConstraint(std::unique_ptr<Constraint> constraint) {
    if (constraint->isStay()) {
		stay_edges_.insert({constraint->getVariables().front(), constraint.get()});
	}
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

Constraint* ConstraintGraph::getConstraintByIndex(size_t i) {
    return constraints_.at(i).get();
}

void ConstraintGraph::markStayDefined(Variable* variable) {
    assert(variable);
    auto constraint = stay_edges_.at(variable);
    constraint->satisfyByMethodIndex(0);
    variable->setForce(constraint->getPriority());
}

int ConstraintGraph::findStayIndex(Variable* variable) {
    assert(variable);
    for (size_t i = 0; i < constraints_.size(); i++) {
        auto constraint = constraints_.at(i).get();
        if (constraint->isStay() && constraint->getVariables().front() == variable) {
            return i;
        }
    }
    throw std::runtime_error("Stay not found");
}

int ConstraintGraph::newStayPriority() {
    static int priority = 1e9;
    return --priority;
}

void ConstraintGraph::processMethods() {
    std::unordered_map<Variable*, bool> visited;
    for (const auto& variable_uptr : variables_) {
        visited[variable_uptr.get()] = false;
    }

    std::vector<Method*> topsort;
    for (const auto& variable_uptr : variables_) {
        if (!visited[variable_uptr.get()]) {
            visitRecursively_(variable_uptr.get(), topsort, visited);
        }
    }

    std::reverse(topsort.begin(), topsort.end());
    for (auto method : topsort) {
        if (method == nullptr) {
            continue;
        }
        method->process();
    }
}

void ConstraintGraph::visitRecursively_(
    Variable* variable, std::vector<Method*>& topsort, std::unordered_map<Variable*, bool>& visited
) {
    assert(variable);
    visited[variable] = true;
    for (auto method : variable->getDependentMethods()) {
        if (!visited.at(method->getOutput())) {
            visitRecursively_(method->getOutput(), topsort, visited);
        }
    }
    topsort.push_back(variable->getDefiningMethod());
}

}  // namespace NSConstraintGraph
