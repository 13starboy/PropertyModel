#pragma once

#include <any>
#include <cstdint>
#include <vector>

namespace ConstraintGraph {

struct Method;

struct Variable
{
    Variable(std::any value);  

    const std::vector<Method*>& getOutputsMethods() const;
    std::vector<Method*> getOutputsMethods();
    
    const std::vector<Method*>& getInputsMethods() const;
    std::vector<Method*> getInputsMethods();

    void addInput(Method* method);
    void removeInput(Method* method);
    void addOutput(Method* method);
    void removeOutput(Method* method); 

    const int64_t getForce() const;
    void setForce(std::int64_t new_force);

    std::any value;
    std::vector<Method*> inputs;
    std::vector<Method*> outputs;
    std::int64_t force = 0;
};

} // namespace ConstraintGraph
