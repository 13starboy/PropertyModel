#pragma once

#include "method.hpp"

#include <vector>
#include <functional>
#include <memory>


namespace ConstraintGraph {


struct Method;

struct Constraint {

    enum class State { Enabled, Disabled };
    
    Constraint(int64_t strength);
    
    void addMethod(std::unique_ptr<Method>&& method);
    
    const Method* getSelectedMethod() const;
    void selectMethod(Method* method);
    void process();

    [[nodiscard]] const std::vector<std::unique_ptr<Method>>& getMethods() const;
    std::vector<std::unique_ptr<Method>>& getMethods();

    const std::int64_t getPriority() const;

    const bool isSatisfied() const;
    void setSatisfied(bool satisfy);
    bool isBlocked();
    void enable();
    void satisfy(Method* method);
    void unsatisfy();

    Method* findMaxPriorMethod();
    
private:
    State state = State::Disabled;
    std::vector<std::unique_ptr<Method>> methods_;
    std::int64_t priority_;
    Method* selected_method_ = nullptr;
    bool is_satisfy_ = false;
};

} // namespace ConstraintGraph
