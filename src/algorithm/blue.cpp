#include "blue.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cassert>

namespace ConstraintGraph {

void DeltaBlueSolver::enableConstraintByIndex(ConstraintGraph& g, size_t index) {
	Constraint* new_constraint = g.getConstraintByIndex(index);
	// bool is_blocked = false;
	// for (const Variable* variable : new_constraint->variables()) {
	// 	if (variable->force_ > new_constraint->priority()) {
	// 		is_blocked = true;
	// 	}
	// }
 
	if (!new_constraint->isBlocked()) {
		new_constraint->enable();
		return;
	}

	Method* chosen_method = (*new_constraint).method(0);
	for (const std::unique_ptr<Method>& method_ptr : new_constraint->methods()) {
		assert(chosen_method->output());
		if (chosen_method->output()->force() < method_ptr->output()->force()) {
			chosen_method = method_ptr.get();
		}
	}

	reverse_path(g, chosen_method->outputs()[0]);
	g[index]->satisfy(chosen_method);

	std::unordered_map<Variable*, int> cycle_visited;
	detect_cycle(g, chosen_method->outputs()[0], cycle_visited);

	std::unordered_map<Variable*, bool> visited;
	recalculate_forces(g, chosen_method->outputs()[0], visited);
}

void DeltaBlueSolver::disableConstraintByIndex(ConstraintGraph& g, size_t index) {
	if (g[index]->is_stay()) {
		disable_stay(g, index);
		return;
	}

	if (!g[index]->is_satisfied()) {
		g[index]->disable();
		return;
	}

	g[index]->unsatisfy();
	Variable* not_defined = g[index]->variables()[0];
	g.define_by_stay(not_defined);

	std::unordered_map<Variable*, bool> visited;
	recalculate_forces(g, not_defined, visited);

	for (int constraint_index = 0; constraint_index < g.constraints_count(); ++constraint_index) {
		if (g[constraint_index]->is_enable() && !g[constraint_index]->is_satisfied()) {
			enable_constraint(g, constraint_index);
		}
	}
}

void DeltaBlueSolver::reverse_path(ConstraintGraph& g, Variable* from) {
	assert(from);
	int end_force = from->force_;
	Variable* current_variable = from;
	std::vector<Method*> path;
	while (current_variable->defining_constraint()->priority() != end_force) {
		for (const std::unique_ptr<Method>& method_ptr : current_variable->defining_constraint()->methods()) {
			Method* method = method_ptr.get();
			if (method->outputs()[0]->force_ == end_force && (!method->is_chosen())) {
				current_variable = method->outputs()[0];
				path.push_back(method);
				break;
			}
		}
	}

	current_variable->defining_constraint()->unsatisfy();
	g.define_by_stay(current_variable);

	std::reverse(path.begin(), path.end());
	for (Method* method : path) {
		method->associated_constraint()->unsatisfy();
		method->associated_constraint()->satisfy(method);
	}
}

void DeltaBlueSolver::recalculate_forces(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, bool>& visited) {
	assert(from);
	visited[from] = true;
	int new_force = from->defining_constraint()->priority();

	for (const std::unique_ptr<Method>& method : from->defining_constraint()->methods()) {
		if (method.get()->outputs()[0] != from) {
			new_force = std::max(new_force, method->outputs()[0]->force_);
		}
	}

	from->force_ = new_force;

	for (const Method* output_method : from->edges_to_methods_) {
		Variable* to = output_method->outputs()[0];
		if (!visited[to]) {
			recalculate_forces(g, to, visited);
		}
	}
}

void DeltaBlueSolver::disable_stay(ConstraintGraph& g, int index) {
	std::unique_ptr<Constraint> new_stay =
		Constraint::make_stay_constraint(g[index]->variables()[0], g.new_stay_priority());
	g.constraints_.push_back(std::move(new_stay));
	enable_constraint(g, g.constraints_.size() - 1);

	g.constraints_[index] = std::move(g.constraints_.back());
	g.stay_constraint_table_[g[index]->variables()[0]] = g.constraints_[index].get();
	g.constraints_.pop_back();
}

void DeltaBlueSolver::detect_cycle(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, int>& visited) {
	assert(from);
	visited[from] = 1;

	for (const Method* output : from->edges_to_methods_) {
		Variable* to = output->outputs()[0];

		if (visited[to] == 1) {
			throw std::logic_error("There is a cycle.");
		} else if (!visited[to]) {
			detect_cycle(g, to, visited);
		}
	}
	visited[from] = 2;
}

} // namespace ConstraintGraph
