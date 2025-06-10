#pragma once

#include <any>
#include <cstdint>
#include <string>
#include <vector>

namespace NSPropertyModel {

struct Method;
struct Constraint;

class Variable {
public:
    Variable(std::any value);

    const std::vector<Method*>& getDependentMethods() const;
    std::vector<Method*> getDependentMethods();
    void addDependentMethod(Method* method);
    void removeDependentMethod(Method* method);

    void setDefiningMethod(Method* method);

    const int getForce() const;
    void setForce(int new_force);

    std::any getValue() const;
    void setValue(std::any value);

    Method* getDefiningMethod();
    Constraint* getDefiningConstraint();

    std::string name;

private:
    std::any value_;
    std::vector<Method*> dependent_methods_;
    Method* defining_method_;
    int force_ = 0;
};

}  // namespace NSPropertyModel
