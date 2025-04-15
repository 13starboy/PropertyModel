#pragma once

#include "constraint.hpp"

#include "variable.hpp"

#include <vector>
#include <memory>


namespace ConstraintGraph {
class ConstraintGraph {
public:
    ConstraintGraph() = default;
    void AddConstraint(Constraint&& constraint);
    void AddConstraint(std::unique_ptr<Constraint> constraint);
    void AddVariable(Variable&& variable);
    void AddVariable(std::unique_ptr<Variable> variable);

    std::vector<std::unique_ptr<Constraint>>& getConstraints();
    [[nodiscard]] const std::vector<std::unique_ptr<Constraint>>& getConstraints() const;

    std::vector<std::unique_ptr<Variable>>& getVariables();
    [[nodiscard]] const std::vector<std::unique_ptr<Variable>>& getVariables() const;

private:
    std::vector<std::unique_ptr<Variable>> variables_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
    std::unordered_map<Constraint*, Variable*> edges_; 
};

} // namespace ConstraintGraph
