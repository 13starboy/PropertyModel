#pragma once

#include "constraint.hpp"

#include "variable.hpp"

#include <vector>
#include <memory>


namespace ConstraintGraph {
class ConstraintGraph {
public:
    ConstraintGraph() = default;
    void addConstraint(Constraint&& constraint);
    void addConstraint(std::unique_ptr<Constraint> constraint);
    void addVariable(Variable&& variable);
    void addVariable(std::unique_ptr<Variable> variable);
    void markStayDefined(Variable* variable);

    std::vector<std::unique_ptr<Constraint>>& getConstraints();
    [[nodiscard]] const std::vector<std::unique_ptr<Constraint>>& getConstraints() const;

    std::vector<std::unique_ptr<Variable>>& getVariables();
    [[nodiscard]] const std::vector<std::unique_ptr<Variable>>& getVariables() const;

    std::unordered_map<Variable*, Constraint*>& getStayEdges();
    [[nodiscard]] const std::unordered_map<Variable*, Constraint*>& getStayEdges() const;

    Constraint* getConstraintByIndex(size_t i);

private:
    std::vector<std::unique_ptr<Variable>> variables_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
    std::unordered_map<Variable*, Constraint*> stay_edges_; 
};

} // namespace ConstraintGraph
