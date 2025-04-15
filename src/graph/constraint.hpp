#pragma once

#include "method.hpp"

#include <vector>
#include <functional>
#include <memory>


namespace ConstraintGraph {

struct Method;

struct Constraint {
    
    Constraint(long long int strength);
    
    void addMethod(std::unique_ptr<Method>&& method);
    
    const Method* getSelectedMethod() const;
    void selectMethod(Method* method);
    void process();

    [[nodiscard]] const std::vector<std::unique_ptr<Method>>& getMethods() const;
    std::vector<std::unique_ptr<Method>>& getMethods();

    const std::int64_t getStrength() const;

    const bool isSatisfied() const;
    void setSatisfied(bool satisfy);
    
private:
    std::vector<std::unique_ptr<Method>> methods_;
    std::int64_t strength_;
    Method* selected_method_ = nullptr;
    bool is_satisfy_ = false;
};

} // namespace ConstraintGraph
