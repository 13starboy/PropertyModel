

#pragma once

#include "algorithm/blue.hpp"
#include "graph/constraint_graph.hpp"
#include "graph/method.hpp"
#include "graph/variable.hpp"
#include "property_model.hpp"


#include <unordered_set>
#include <queue>
#include <limits>
#include <string>

namespace ConstraintGraph {

class Builder{

  public:
	Builder() noexcept = default;

    void addVariable(Type type, std::string& name, std::any value) {
        property_model_->add_variable(type, name, value);
    }

    void addConstraint(std::vector<std::string>& variables, int64_t priority, bool enable) {
        if (current_constraint_) {
            property_model_->add_constraint(std::move(current_constraint_));
        }

        auto refs = property_model_->bind_variables(variables);
        current_constraint_ = std::make_unique<Constraint>(
            refs, 
            priority, 
            enable ? Status::Enabled : Status::Disabled
        );
    }

    void addMethod(
        std::function<void(std::any& output, const std::vector<std::any>&)> function,
        const std::string& output,
        const std::vector<std::string>& inputs)
    {
        auto method = property_model_->bind_method(function, output, inputs);
        auto input_refs = property_model_->bind_variables(inputs);
        auto output_refs = property_model_->bind_variables(output);
        
        current_constraint_->add_method(
            std::make_unique<Method>(
                method, 
                input_refs, 
                output_refs, 
                current_constraint_.get()
            )
        );
    }

    void removeVariable(const std::string& name) {
        auto it = variables_.find(name);
        if (it == variables_.end()) {
            return;
        }

        remove_from_category(Belong::DataVariable, name);
        remove_from_category(Belong::ValueVariable, name);
        remove_from_category(Belong::OutputVariable, name);

        constraint_graph_.remove_variable(it->second);

        variables_.erase(it);
    }

    void removeConstraint(size_t index) {
        constraint_graph_.remove_constraint(index);
    }

    std::unique_ptr<PropertyModel> extract() {
        if (current_constraint_) {
            property_model_->add_constraint(std::move(current_constraint_));
        }

        property_model_->add_stay_constraints();
        property_model_->prepare_solution_graph();
        return std::move(property_model_);
    }

  private:
	PropertyModel_ property_model_;
	std::unique_ptr<Constraint> current_constraint_;

};

} // namespace ConstraintGraph