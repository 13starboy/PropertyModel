#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <vector>

#include <pmodel/graph/method.hpp>

namespace NSPropertyModel {

struct Constraint {
    Constraint(const std::vector<Variable*>& variables, int priority);

    static std::unique_ptr<Constraint> buildRegularConstraint(const std::vector<Variable*>& variables, int priority) {
        return std::make_unique<Constraint>(variables, priority);
    }

    static std::unique_ptr<Constraint> buildStayConstraint(Variable* variable, int priority) {
        assert(variable);
        std::vector<Variable*> stay_variable = {variable};
        auto new_stay = std::make_unique<Constraint>(stay_variable, priority);
        new_stay->is_stay_ = true;
        new_stay->is_enabled_ = true;

        new_stay->addMethod(
            std::make_unique<Method>([]() { return; }, std::vector<Variable*>(), variable, new_stay.get())
        );

        return new_stay;
    }

    void addMethod(std::unique_ptr<Method>&& method);
    const std::vector<std::unique_ptr<Method>>& getMethods() const;

    const std::vector<Variable*>& getVariables() const;
    std::vector<Variable*> getVariables();

    void selectMethod(Method* method);
    void processSelectedMethod();
    const Method* getSelectedMethod() const;

    int getPriority() const;

    bool isSatisfied() const;
    bool isStay() const;
    bool isEnabled() const;
    bool isBlocked() const;

    void enable();
    void disable();

    void satisfyByMethodIndex(int index);
    void satisfy(Method* method);
    void unsatisfy();

    Method* findMaxPriorMethod();

private:
    std::vector<Variable*> variables_;
    int priority_;

    std::vector<std::unique_ptr<Method>> methods_;
    Method* selected_method_ = nullptr;

    bool is_enabled_ = true;
    bool is_satisfied_ = false;
    bool is_stay_ = false;
};

}  // namespace NSPropertyModel
