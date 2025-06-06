#pragma once

#include <functional>
#include <vector>

namespace NSPropertyModel {

struct Variable;

struct Constraint;

struct Method {
    Method(std::function<void()> func, std::vector<Variable*> inputs, Variable* output, Constraint* parent);
    void process() const;

    Constraint* getConstraint();
    const Constraint* getConstraint() const;
    Variable* getOutput();
    const Variable* getOutput() const;
    std::vector<Variable*> getInputs();
    const std::vector<Variable*> getInputs() const;
    bool isSelected();
    void satisfy();
    void unsatisfy();

    std::function<void()> function;
    Constraint* parent;
    std::vector<Variable*> inputs;
    Variable* output;
};

}  // namespace NSPropertyModel
