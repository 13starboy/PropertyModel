

#pragma once

#include "algorithm/blue.hpp"
#include "graph/constraint_graph.hpp"
#include "graph/method.hpp"
#include "graph/variable.hpp"


#include <unordered_set>
#include <queue>
#include <limits>
#include <string>

namespace ConstraintGraph {

class PropertyModel{
  public:

	void set(std::string& name, std::any value) {
        Variable* variable = variables_[name];
        
        DeltaBlueSolver::disableConstraintByIndex(constraint_graph_, 
                                    constraint_graph_.find_index_of_stay(variable));
        try {
            variable->load_data(std::move(value));
        } 
        catch (const std::bad_any_cast& e) {
            throw std::runtime_error("Type mismatch in variable " + name + ": " + e.what());
        }
        
        constraint_graph_.update_values();
    }

	void enableConstraint(size_t index) {
		DeltaBlueSolver::enableConstraintByIndex(constraint_graph_, index);
		constraint_graph_.updateValues();
	}

	void disableConstraint(size_t index) {
		DeltaBlueSolver::disableConstraintByIndex(constraint_graph_, index);
		constraint_graph_.updateValues();
	}


  private:

    enum class Type {
        DataVariable,
        ValueVariable,
        OutputVariable
    };  


	void add_variable(Type belong, std::string name, std::any value) {
        Variable* pointer = constraint_graph_.addVariable(belong, name, value);
        variables_.insert({name, pointer});

        switch (belong) {
            case Type::DataVariable:
                data_.push_back(std::move(name));
                break;
            case Type::ValueVariable:
                value_.push_back(std::move(name));
                break;
            case Type::OutputVariable:
                output_.push_back(std::move(name));
                break;
        }
    }

	std::vector<Variable*> bind_variables(const std::vector<std::string>& names) {
		std::vector<Variable*> references;
		references.reserve(names.size());
		for (const std::string& name : names) {
			references.push_back(variables_[name]);
		}
		return references;
	}


    std::function<void()> bind_method(
        std::function<std::vector<std::any>(const std::vector<std::any>&)> function,
        std::string& output,
        std::vector<std::string>& inputs
    ) {
        std::vector<Variable*> input_variables = bind_variables(inputs);
        std::vector<Variable*> output_variables = bind_variables(outputs);

        std::function<void()> method = [this, function = std::move(function), inputs = std::move(inputs), outputs = std::move(outputs)]() {
            // Получаем входные аргументы
            std::vector<std::any> args;
            for (const auto& input : inputs) {
                args.push_back(variables_[input]->get_data());
            }

            // Вызываем функцию и получаем результат
            std::vector<std::any> result = function(args);

            // Сохраняем результаты в выходные переменные
            for (size_t i = 0; i < result.size() && i < outputs.size(); ++i) {
                variables_[outputs[i]]->load_data(result[i]);
            }
        };

        return method;
    }

	void add_constraint(std::unique_ptr<Constraint>&& constraint) {
		constraint_graph_.add_constraint(std::move(constraint));
	}

	void add_stay_constraints() {
		for (const auto& [name, variable] : variables_) {
			constraint_graph_.add_constraint(
				Constraint::make_stay_constraint(variable, constraint_graph_.new_stay_priority()));
			constraint_graph_.define_by_stay(variable);
		}
	}

	void prepare_solution_graph() {
		for (int constraint_index = 0; constraint_index < constraint_graph_.constraints_count(); ++constraint_index) {
			Constraint* current_constraint = constraint_graph_[constraint_index];
			if (current_constraint->is_enable() && !current_constraint->is_stay()) {
				DeltaBlue::enable_constraint(constraint_graph_, constraint_index);
			}
		}
	}

	std::vector<std::string> data_;
	std::vector<std::string> value_;
	std::vector<std::string> output_;
	std::unordered_map<std::string, Variable*> variables_;
	ConstraintGraph constraint_graph_;
};

} // namespace ConstraintGraph