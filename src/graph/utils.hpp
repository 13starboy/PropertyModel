#pragma once
#include "constraint_graph.hpp"

#include <map>
#include <string>

namespace NSConstraintGraph {

class GraphPrinter {
public:
    static void printGraph(const ConstraintGraph& graph);

private:
    static void printHeader();
    static void printFooter();
    static void printVariables(const ConstraintGraph& graph,
                             const std::map<const Variable*, std::string>& var_names,
                             const std::map<const Constraint*, std::string>& constr_names);
    static void printConstraints(const ConstraintGraph& graph,
                               const std::map<const Variable*, std::string>& var_names,
                               const std::map<const Constraint*, std::string>& constr_names);
};

} // namespace NSConstraintGraph
