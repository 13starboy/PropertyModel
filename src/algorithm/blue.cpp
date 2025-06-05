#include "blue.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cassert>

namespace ConstraintGraph {

void DeltaBlueSolver::enableConstraintByIndex(ConstraintGraph& g, size_t index) {
	Constraint* new_constraint = g.getConstraintByIndex(index);
 
	if (!new_constraint->isBlocked()) {
		new_constraint->enable();
		return;
	}

	auto chosen_method = new_constraint->findMaxPriorMethod();

	reversePath(g, chosen_method->getOutput());
	g.getConstraintByIndex(index)->satisfy(chosen_method);

	std::unordered_map<Variable*, int> check;
	detectCycle(check, g, chosen_method->getOutput());

	std::unordered_map<Variable*, bool> visited;
	recalculateForces(visited, g, chosen_method->getOutput());
}

void DeltaBlueSolver::disableConstraintByIndex(ConstraintGraph& g, size_t index) {
	if (g.getConstraintByIndex(index)->is_stay()) {
		disableStay(g, index);
		return;
	}

	if (!g.getConstraintByIndex(index)->is_satisfied()) {
		g.getConstraintByIndex(index)->disable();
		return;
	}

	g.getConstraintByIndex(index)->unsatisfy();
	Variable* not_defined = g.getConstraintByIndex(index)->variables()[0];
	g.define_by_stay(not_defined);

	std::unordered_map<Variable*, bool> visited;
	recalculateForces(visited, g, not_defined);

	for (int i = 0; i < g.constraints_count(); ++i) {
		if (g.getConstraintByIndex(i)->is_enable() && !g.getConstraintByIndex(i)->is_satisfied()) {
			enable_constraint(g, i);
		}
	}
}

void DeltaBlueSolver::reversePath(ConstraintGraph& g, Variable* v) {
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
}

void DeltaBlueSolver::recalculateForces(std::unordered_map<Variable*, bool>& visited, ConstraintGraph& g, Variable* from) {
	assert(from);
	visited[from] = true;
	int64_t max_force = from->getDefiningConstraint()->getPriority();

	for (const auto& method : from->getDefiningConstraint()->getMethods()) {
		if (method.get()->getOutput() != from) {
			max_force = std::max(max_force, method->getOutput()->getForce());
		}
	}

	from->setForce(max_force);

	for (const auto output_method : from->getOutputsMethods()) {
		Variable* to = output_method->getOutput();
		if (!visited[to]) {
			recalculateForces(visited, g, to);
		}
	}
}

void DeltaBlueSolver::disableStay(ConstraintGraph& g, int index) {
	std::unique_ptr<Constraint> new_stay =
		Constraint::make_stay_constraint(g.getConstraintByIndex(index)->variables()[0], g.new_stay_priority());
	g.addConstraint(std::move(new_stay));

	enableConstraintByIndex(g, g.getConstraints().size() - 1);

	g.getConstraints()[index] = std::move(g.getConstraints().back());
	g.getStayEdges()[g.getConstraintByIndex(index)->variables()[0]] = g.getConstraints()[index].get();
	g.getConstraints().pop_back();
}

void DeltaBlueSolver::detectCycle(std::unordered_map<Variable*, int>& visited, ConstraintGraph& g, Variable* from) {
	assert(from);
	visited[from] = 1;

	for (auto output : from->getOutputsMethods()) {
		Variable* to = output->getOutput();
		if (visited[to] == 1) {
			throw std::logic_error("Cycle found");
		} else if (!visited[to]) {
			detectCycle(visited, g, to);
		}
	}
	visited[from] = 2;
}

} // namespace ConstraintGraph
