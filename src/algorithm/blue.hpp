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
        void recalculateForces(std::unordered_map<Variable*, bool>& visited, ConstraintGraph& g, Variable* from);
        void detectCycle(std::unordered_map<Variable*, int>& visited, ConstraintGraph& g, Variable* from);
        void reversePath(ConstraintGraph& g, Variable* from);
        void disableStay(ConstraintGraph& g, int index);
    };

} // namespace ConstraintGraph
