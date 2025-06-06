

#pragma once

#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <pmodel/algorithm/blue.hpp>
#include <pmodel/graph/constraint_graph.hpp>
#include <pmodel/graph/method.hpp>
#include <pmodel/graph/variable.hpp>

namespace NSPropertyModel {

class PropertyModel {
public:
    enum class Type { DataVariable, ValueVariable, OutputVariable };
    void set(const std::string& name, std::any value);
    std::any get(const std::string& name);


    void enableConstraint(size_t index);

    void disableConstraint(size_t index);

    ConstraintGraph constraint_graph_;

private:
    void addVariable(Type belong, const std::string& name, std::any value);
    std::vector<Variable*> bindVariables(const std::vector<std::string>& names);

    std::function<void()> bindMethod(
        std::function<std::any(const std::vector<std::any>&)> function, const std::vector<std::string>& inputs,
        const std::string& output
    );

    void addConstraint(std::unique_ptr<Constraint>&& constraint);

    void addStayConstraints();
    void prepareSolutionGraph();

    std::vector<std::string> data_;
    std::vector<std::string> value_;
    std::vector<std::string> output_;
    std::unordered_map<std::string, Variable*> variables_;
    // ConstraintGraph constraint_graph_;

    friend class Builder;
};

}  // namespace NSPropertyModel
