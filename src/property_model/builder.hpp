

#pragma once

#include <limits>
#include <queue>
#include <string>
#include <unordered_set>

#include "algorithm/blue.hpp"
#include "graph/constraint_graph.hpp"
#include "graph/method.hpp"
#include "graph/variable.hpp"
#include "property_model.hpp"

namespace NSConstraintGraph {

class Builder {
public:
    Builder();

    void addVariable(PropertyModel::Type type, const std::string& name, std::any value);

    void addConstraint(const std::vector<std::string>& variables, int priority, bool enabled);

    void addMethod(
        std::function<std::any(const std::vector<std::any>&)> function, const std::vector<std::string>& inputs,
        const std::string& output
    );

    void removeVariable(const std::string& name);

    void removeConstraint(size_t index);

    std::unique_ptr<PropertyModel> extract();

private:
    std::unique_ptr<PropertyModel> propertyModel;
    std::unique_ptr<Constraint> currentConstraint;
};

}  // namespace NSConstraintGraph
