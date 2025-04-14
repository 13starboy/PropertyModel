#pragma once

#include <vector>
#include <any>

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

    std::any value;
    std::vector<Method*> inputs;
    std::vector<Method*> outputs;
};

} // namespace ConstraintGraph
