#pragma once

#include <memory>
#include <vector>

#include "constraint.hpp"
#include "variable.hpp"

namespace NSConstraintGraph {
class ConstraintGraph {
public:
    ConstraintGraph() = default;
    void addConstraint(std::unique_ptr<Constraint> constraint);
    void addVariable(Variable&& variable);
    void addVariable(std::unique_ptr<Variable> variable);
    void markStayDefined(Variable* variable);

    int newStayPriority();
    int findStayIndex(Variable* variable);
    void processMethods();

    std::vector<std::unique_ptr<Constraint>>& getConstraints();
    [[nodiscard]] const std::vector<std::unique_ptr<Constraint>>& getConstraints() const;

    std::vector<std::unique_ptr<Variable>>& getVariables();
    [[nodiscard]] const std::vector<std::unique_ptr<Variable>>& getVariables() const;

    std::unordered_map<Variable*, Constraint*>& getStayEdges();
    [[nodiscard]] const std::unordered_map<Variable*, Constraint*>& getStayEdges() const;

    Constraint* getConstraintByIndex(size_t i);

private:
    void visitRecursively_(
        Variable* variable, std::vector<Method*>& topsort, std::unordered_map<Variable*, bool>& visited
    );

    std::vector<std::unique_ptr<Variable>> variables_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
    std::unordered_map<Variable*, Constraint*> stay_edges_;
};

}  // namespace NSConstraintGraph
