#pragma once

#include "graph/constraint_graph.hpp"
#include "graph/method.hpp"

#include <unordered_set>
#include <queue>
#include <limits>

namespace ConstraintGraph {

class DeltaBlueSolver {
    public:
        void enableConstraint(ConstraintGraph& graph, Constraint* constraint);
        void disableСonstraint(ConstraintGraph& graph, Constraint* constraint);
        void enableConstraintByIndex(ConstraintGraph& g, size_t index);
        void disableConstraintByIndex(ConstraintGraph& g, size_t index);
    
    private:
        void clear_state(ConstraintGraph& graph);
        void update_forces(ConstraintGraph& graph, Variable* output);
        void recalculate_forces(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, bool>& visited);
        void detect_cycle(ConstraintGraph& g, Variable* from, std::unordered_map<Variable*, int>& visited);
        void reverse_path(ConstraintGraph& g, Variable* from);
        void disable_stay(ConstraintGraph& g, int index);
    };

} // namespace ConstraintGraph
