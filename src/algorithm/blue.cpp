#include "blue.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cassert>

namespace ConstraintGraph {


void DeltaBlueSolver::solve(ConstraintGraph& graph) {
    clear_state(graph);
    
    auto& constraints = graph.getConstraints();
    std::vector<Constraint*> sorted_constraints;
    sorted_constraints.reserve(constraints.size());
    
    for (auto& c : constraints) {
        if (!c->isSatisfied()) {
            sorted_constraints.push_back(c.get());
        }
    }
    
    std::sort(sorted_constraints.begin(), sorted_constraints.end(),
        [](Constraint* a, Constraint* b) {
            return a->getStrength() < b->getStrength();
        });
    
    for (auto* constraint : sorted_constraints) {
        if (!constraint->isSatisfied()) {
            enable_constraint(graph, constraint);
        }
    }
}

void DeltaBlueSolver::enable_constraint(ConstraintGraph& graph, Constraint* constraint) {
    for (auto& method : constraint->methods()) {
        bool inputs_ready = true;
        for (auto* input : method->inputs()) {
            if (!input || !input->is_defined()) {
                inputs_ready = false;
                break;
            }
        }
        
        if (inputs_ready) {
            constraint->satisfy(method.get());
            method->execute();
            
            std::unordered_map<Variable*, bool> visited;
            recalculate_forces(graph, method->output(), visited);
            
            std::unordered_map<Variable*, int> cycle_visited;
            detect_cycle(graph, method->output(), cycle_visited);
            
            return;
        }
    }
}

void DeltaBlueSolver::disable_constraint(ConstraintGraph& graph, Constraint* constraint) {
    if (!constraint->is_satisfied()) return;
    
    constraint->unsatisfy();
    if (auto* output = constraint->selected_method()->output()) {
        output->mark_undefined();
    }
    
    for (auto& c : graph.getConstraints()) {
        if (!c->is_satisfied()) {
            enable_constraint(graph, c.get());
        }
    }
}

void DeltaBlueSolver::clear_state(ConstraintGraph& graph) {
    for (auto& var : graph.getVariables()) {
        var->reset_force();
    }
}

void DeltaBlueSolver::recalculate_forces(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, bool>& visited) {
    assert(from);
    visited[from] = true;
    int new_force = from->defining_constraint()->priority();

    for (const auto& method : from->defining_constraint()->methods()) {
        if (method->outputs()[0] != from) {
            new_force = std::max(new_force, method->outputs()[0]->force());
        }
    }

    from->set_force(new_force);

    for (const Method* output_method : from->edges_to_methods()) {
        Variable* to = output_method->outputs()[0];
        if (!visited[to]) {
            recalculate_forces(g, to, visited);
        }
    }
}

void DeltaBlueSolver::detect_cycle(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, int>& visited) {
    assert(from);
    visited[from] = 1;

    for (const Method* output : from->edges_to_methods()) {
        Variable* to = output->outputs()[0];

        if (visited[to] == 1) {
            throw std::logic_error("Cycle detected in constraint graph");
        } else if (!visited[to]) {
            detect_cycle(g, to, visited);
        }
    }
    visited[from] = 2;
}

void DeltaBlueSolver::reverse_path(ConstraintGraph& g, Variable* from) {
    assert(from);
    int end_force = from->force();
    Variable* current_variable = from;
    std::vector<Method*> path;
    
    while (current_variable->defining_constraint()->priority() != end_force) {
        for (const auto& method_ptr : current_variable->defining_constraint()->methods()) {
            Method* method = method_ptr.get();
            if (method->outputs()[0]->force() == end_force && (!method->is_chosen())) {
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

void DeltaBlueSolver::disable_stay(ConstraintGraph& g, int index) {
    std::unique_ptr<Constraint> new_stay =
        Constraint::make_stay_constraint(g[index]->variables()[0], g.new_stay_priority());
    g.constraints().push_back(std::move(new_stay));
    enable_constraint(g, g.constraints().back().get());

    g.constraints()[index] = std::move(g.constraints().back());
    g.stay_constraint_table()[g[index]->variables()[0]] = g.constraints()[index].get();
    g.constraints().pop_back();
}

} // namespace ConstraintGraph
