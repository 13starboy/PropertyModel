#include "utils.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

namespace ConstraintGraph {

void GraphPrinter::printGraph(const ConstraintGraph& graph) {
    std::map<const Variable*, std::string> var_names;
    std::map<const Constraint*, std::string> constr_names;
    
    int var_counter = 1;
    for (const auto& var : graph.getVariables()) {
        var_names[var.get()] = "v" + std::to_string(var_counter++);
    }
    
    int constr_counter = 1;
    for (const auto& constr : graph.getConstraints()) {
        if (constr->getStrength() < 0) {
            constr_names[constr.get()] = "stay" + std::to_string(constr_counter++);
        } else {
            constr_names[constr.get()] = "c" + std::to_string(constr_counter++);
        }
    }

    printHeader();
    printVariables(graph, var_names, constr_names);
    printConstraints(graph, var_names, constr_names);
    printFooter();
}

void GraphPrinter::printHeader() {
    std::cout << "----------------------------------------\n";
    std::cout << "|        CONSTRAINT GRAPH STATE        |\n";
    std::cout << "|--------------------------------------|\n";
}

void GraphPrinter::printFooter() {
    std::cout << "----------------------------------------\n";
}

void GraphPrinter::printVariables(const ConstraintGraph& graph, 
                                const std::map<const Variable*, std::string>& var_names,
                                const std::map<const Constraint*, std::string>& constr_names) {
    std::cout << "|  VARIABLES:\n";
    
    for (const auto& var : graph.getVariables()) {
        const auto& name = var_names.at(var.get());
        
        std::cout << "|    " << name << " = ";
        try {
            if (var->value.type() == typeid(int)) {
                std::cout << std::any_cast<int>(var->value);
            } 
            else if (var->value.type() == typeid(double)) {
                std::cout << std::fixed << std::setprecision(2) 
                         << std::any_cast<double>(var->value);
            }
            else if (var->value.type() == typeid(float)) {
                std::cout << std::fixed << std::setprecision(2) 
                         << std::any_cast<float>(var->value);
            }
            else {
                std::cout << "?";
            }
        } catch (const std::bad_any_cast&) {
            std::cout << "unset";
        }
        
        std::cout << " (priority: " << var->getPriority() << ")\n";
        
        if (!var->getInputsMethods().empty()) {
            std::cout << "|      <- from: ";
            bool first = true;
            for (auto method : var->getInputsMethods()) {
                if (!first) std::cout << ", ";
                first = false;
                std::cout << constr_names.at(method->getConstrain());
            }
            std::cout << "\n";
        }

        if (!var->getOutputsMethods().empty()) {
            std::cout << "|      -> to: ";
            bool first = true;
            for (auto method : var->getOutputsMethods()) {
                if (!first) std::cout << ", ";
                first = false;
                std::cout << constr_names.at(method->getConstrain());
            }
            std::cout << "\n";
        }
    }
}

void GraphPrinter::printConstraints(const ConstraintGraph& graph,
                                  const std::map<const Variable*, std::string>& var_names,
                                  const std::map<const Constraint*, std::string>& constr_names) {
    std::cout << "|\n|  CONSTRAINTS:\n";
    
    for (const auto& constr : graph.getConstraints()) {
        const auto& name = constr_names.at(constr.get());
        
        std::cout << "|    " << name 
                  << " [strength: " << constr->getStrength()
                  << ", " << (constr->isSatisfied() ? "SATISFIED" : "PENDING") 
                  << "]\n";
    
        if (constr->getSelectedMethod()) {
            std::cout << "|      *active method*\n";
        }
        
        for (const auto& method : constr->getMethods()) {
            std::cout << "|      ";
            if (!method->getInputs().empty()) {
                bool first = true;
                for (auto input : method->getInputs()) {
                    if (!first) std::cout << ", ";
                    first = false;
                    std::cout << var_names.at(input);
                }
            } else {
                std::cout << "none";
            }
            
            std::cout << " -> " << var_names.at(method->getOutput()) << "\n";
        }
    }
}

} // namespace ConstraintGraph