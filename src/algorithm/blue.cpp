#include "blue.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_set>

namespace NSConstraintGraph::DeltaBlueSolver {

void enableConstraintByIndex(ConstraintGraph& g, size_t index) {
    Constraint* new_constraint = g.getConstraintByIndex(index);

    if (!new_constraint->isBlocked()) {
        new_constraint->enable();
        return;
    }

    auto chosen_method = new_constraint->findMaxPriorMethod();

    reversePath(g, chosen_method->getOutput());
    g.getConstraintByIndex(index)->satisfy(chosen_method);

    std::unordered_map<Variable*, int> detect_cycle_visited;
    detectCycle(detect_cycle_visited, g, chosen_method->getOutput());

    std::unordered_map<Variable*, bool> visited;
    recalculateForces(visited, g, chosen_method->getOutput());
}

void disableConstraintByIndex(ConstraintGraph& g, size_t index) {
    if (g.getConstraintByIndex(index)->isStay()) {
        disableStay(g, index);
        return;
    }

    if (!g.getConstraintByIndex(index)->isSatisfied()) {
        g.getConstraintByIndex(index)->disable();
        return;
    }

    g.getConstraintByIndex(index)->unsatisfy();
    Variable* not_defined = g.getConstraintByIndex(index)->getVariables().front();
    g.markStayDefined(not_defined);

    std::unordered_map<Variable*, bool> visited;
    recalculateForces(visited, g, not_defined);

    for (int i = 0; i < g.getConstraints().size(); ++i) {
        if (g.getConstraintByIndex(i)->isEnabled() && !g.getConstraintByIndex(i)->isSatisfied()) {
            enableConstraintByIndex(g, i);
        }
    }
}

void reversePath(ConstraintGraph& g, Variable* v) {
    assert(v);
    int end_force = v->getForce();
    Variable* current_variable = v;
    std::vector<Method*> path;
    while (current_variable->getDefiningConstraint()->getPriority() != end_force) {
        for (const auto& current_method : current_variable->getDefiningConstraint()->getMethods()) {
            Method* method = current_method.get();
            if (method->getOutput()->getForce() == end_force && (!method->isSelected())) {
                current_variable = method->getOutput();
                path.push_back(method);
                break;
            }
        }
    }

    current_variable->getDefiningConstraint()->unsatisfy();
	g.markStayDefined(current_variable);

	std::reverse(path.begin(), path.end());
	for (Method* method : path) {
		method->getConstraint()->unsatisfy();
		method->getConstraint()->satisfy(method);
	}
}

void recalculateForces(std::unordered_map<Variable*, bool>& visited, ConstraintGraph& g, Variable* from) {
    assert(from);
    visited[from] = true;
    int max_force = from->getDefiningConstraint()->getPriority();

    for (const auto& method : from->getDefiningConstraint()->getMethods()) {
        if (method.get()->getOutput() != from) {
            max_force = std::max(max_force, method->getOutput()->getForce());
        }
    }

    from->setForce(max_force);

    for (const auto dependent_method : from->getDependentMethods()) {
        Variable* to = dependent_method->getOutput();
        if (!visited[to]) {
            recalculateForces(visited, g, to);
        }
    }
}

void disableStay(ConstraintGraph& g, int index) {
    std::unique_ptr<Constraint> new_stay = Constraint::buildStayConstraint(
        g.getConstraintByIndex(index)->getVariables().front(), g.newStayPriority()
    );
    g.addConstraint(std::move(new_stay));

    enableConstraintByIndex(g, g.getConstraints().size() - 1);

    g.getConstraints()[index] = std::move(g.getConstraints().back());
    g.getStayEdges()[g.getConstraintByIndex(index)->getVariables().front()] = g.getConstraints()[index].get();
    g.getConstraints().pop_back();
}

void detectCycle(std::unordered_map<Variable*, int>& visited, ConstraintGraph& g, Variable* from) {
    assert(from);
    visited[from] = 1;

    for (auto output : from->getDependentMethods()) {
        Variable* to = output->getOutput();
        if (visited[to] == 1) {
            throw std::logic_error("Cycle found");
        } else if (!visited[to]) {
            detectCycle(visited, g, to);
        }
    }
    visited[from] = 2;
}

}  // namespace NSConstraintGraph::DeltaBlueSolver
