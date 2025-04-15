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

    const int getPriority() const;
    void setPriority(std::int64_t priority);

    std::any value;
    std::vector<Method*> inputs;
    std::vector<Method*> outputs;
    std::int64_t priority = 0;
};

} // namespace ConstraintGraph
