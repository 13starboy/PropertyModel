#pragma once

#include <method.hpp>
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

    [[nodiscard]] const std::vector<std::unique_ptr<Method>>& getMethods() const;
    std::vector<std::unique_ptr<Method>>& getMethods();
    
private:
    std::vector<std::unique_ptr<Method>> methods_;
    long long int strength_;
    Method* selected_method_ = nullptr;
};

} // namespace ConstraintGraph
