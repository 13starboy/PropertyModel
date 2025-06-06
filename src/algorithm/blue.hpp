#pragma once

#include <limits>
#include <queue>
#include <unordered_set>

#include "graph/constraint_graph.hpp"
#include "graph/method.hpp"

namespace NSConstraintGraph::DeltaBlueSolver {
void enableConstraintByIndex(ConstraintGraph& g, size_t index);
void disableConstraintByIndex(ConstraintGraph& g, size_t index);

void recalculateForces(std::unordered_map<Variable*, bool>& visited, ConstraintGraph& g, Variable* from);
void detectCycle(std::unordered_map<Variable*, int>& visited, ConstraintGraph& g, Variable* from);
void reversePath(ConstraintGraph& g, Variable* from);
void disableStay(ConstraintGraph& g, int index);
}  // namespace NSConstraintGraph::DeltaBlueSolver
