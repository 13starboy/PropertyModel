#include "utils.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace NSPropertyModel {

void GraphPrinter::printGraph(const ConstraintGraph& graph) {
    std::map<const Variable*, std::string> var_names;
    std::map<const Constraint*, std::string> constr_names;

    int var_counter = 1;
    for (const auto& var : graph.getVariables()) {
        var_names[var.get()] = "v" + std::to_string(var_counter++);
    }

    int constr_counter = 1;
    for (const auto& constr : graph.getConstraints()) {
        if (constr->getPriority() < 0) {
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

void GraphPrinter::printVariables(
    const ConstraintGraph& graph, const std::map<const Variable*, std::string>& var_names,
    const std::map<const Constraint*, std::string>& constr_names
) {
    std::cout << "|  VARIABLES:\n";

    for (const auto& var : graph.getVariables()) {
        const auto& name = var_names.at(var.get());

        std::cout << "|    " << name << " = ";
        try {
            if (var->getValue().type() == typeid(int)) {
                std::cout << std::any_cast<int>(var->getValue());
            } else if (var->getValue().type() == typeid(double)) {
                std::cout << std::fixed << std::setprecision(2) << std::any_cast<double>(var->getValue());
            } else if (var->getValue().type() == typeid(float)) {
                std::cout << std::fixed << std::setprecision(2) << std::any_cast<float>(var->getValue());
            } else if (var->getValue().type() == typeid(std::string)) {
                std::cout << std::any_cast<std::string>(var->getValue());
            } else if (var->getValue().type() == typeid(bool)) {
                std::cout << std::any_cast<bool>(var->getValue());
            } else {
                std::cout << "?";
            }
        } catch (const std::bad_any_cast&) {
            std::cout << "unset";
        }

        std::cout << " (priority: " << var->getForce() << ")\n";

        if (var->getDefiningMethod()) {
            std::cout << "|      <- from: ";
            bool first = true;
            auto method = var->getDefiningMethod();
            if (!first)
                std::cout << ", ";
            first = false;
            std::cout << constr_names.at(method->getConstraint());
            std::cout << "\n";
        }

        if (!var->getDependentMethods().empty()) {
            std::cout << "|      -> to: ";
            bool first = true;
            for (auto method : var->getDependentMethods()) {
                if (!first)
                    std::cout << ", ";
                first = false;
                std::cout << constr_names.at(method->getConstraint());
            }
            std::cout << "\n";
        }
    }
}

void GraphPrinter::printConstraints(
    const ConstraintGraph& graph, const std::map<const Variable*, std::string>& var_names,
    const std::map<const Constraint*, std::string>& constr_names
) {
    std::cout << "|\n|  CONSTRAINTS:\n";

    for (const auto& constr : graph.getConstraints()) {
        const auto& name = constr_names.at(constr.get());

        std::cout << "|    " << name << " [strength: " << constr->getPriority() << ", "
                  << (constr->isSatisfied() ? "SATISFIED" : "PENDING") << "]\n";

        if (constr->getSelectedMethod()) {
            std::cout << "|      *active method*  ";
            bool first = true;
            for (auto input : constr->getSelectedMethod()->getInputs()) {
                if (!first)
                    std::cout << ", ";
                first = false;
                std::cout << var_names.at(input);
            }

            std::cout << " -> " << var_names.at(constr->getSelectedMethod()->getOutput()) << "\n";
            ;
        }

        for (const auto& method : constr->getMethods()) {
            std::cout << "|      ";
            if (!method->getInputs().empty()) {
                bool first = true;
                for (auto input : method->getInputs()) {
                    if (!first)
                        std::cout << ", ";
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

}  // namespace NSPropertyModel
