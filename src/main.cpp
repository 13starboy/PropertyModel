#include "graph/constraint_graph.hpp"
#include "algorithm/blue.hpp"
#include "utils.hpp"

#include <iostream>
#include <memory>



int main() {

    try {

        ConstraintGraph::ConstraintGraph graph;
        ConstraintGraph::DeltaBlueSolver solver;

        std::cout << "=== Creating variables ===" << std::endl;

        std::unique_ptr<ConstraintGraph::Variable> var_a = std::make_unique<ConstraintGraph::Variable>(10.0);
        std::unique_ptr<ConstraintGraph::Variable> var_b = std::make_unique<ConstraintGraph::Variable>(0.0);
        std::unique_ptr<ConstraintGraph::Variable> var_c = std::make_unique<ConstraintGraph::Variable>(0.0);
        std::unique_ptr<ConstraintGraph::Variable> var_d = std::make_unique<ConstraintGraph::Variable>(5.0);


        ConstraintGraph::Variable* a = var_a.get();
        ConstraintGraph::Variable* b = var_b.get();
        ConstraintGraph::Variable* c = var_c.get();
        ConstraintGraph::Variable* d = var_d.get();

        graph.AddVariable(std::move(var_a));
        graph.AddVariable(std::move(var_b));
        graph.AddVariable(std::move(var_c));
        graph.AddVariable(std::move(var_d));
        

        std::cout << "Initial state:" << std::endl;
        std::cout << "a = " << std::any_cast<double>(a->value) << std::endl;
        std::cout << "b = " << std::any_cast<double>(b->value) << std::endl;
        std::cout << "c = " << std::any_cast<double>(c->value) << std::endl;
        std::cout << "d = " << std::any_cast<double>(d->value) << std::endl;


        std::cout << "\n=== Adding constraints ===" << std::endl;
        
        auto constraint1 = std::make_unique<ConstraintGraph::Constraint>(100); 
        auto method1 = std::make_unique<ConstraintGraph::Method>(
            [&a]() { return std::any_cast<double>(a->value) * 2.0; },
            std::vector<ConstraintGraph::Variable*>{a},
            b,
            constraint1.get()
        );
        constraint1->addMethod(std::move(method1));
        graph.AddConstraint(std::move(constraint1));
        std::cout << "Added constraint: b = a * 2" << std::endl;


        auto constraint2 = std::make_unique<ConstraintGraph::Constraint>(200);
        auto method2 = std::make_unique<ConstraintGraph::Method>(
            [&b, &d]() { 
                return std::any_cast<double>(b->value) + std::any_cast<double>(d->value); 
            },
            std::vector<ConstraintGraph::Variable*>{b, d},
            c,
            constraint2.get()
        );
        constraint2->addMethod(std::move(method2));
        graph.AddConstraint(std::move(constraint2));
        std::cout << "Added constraint: c = b + d" << std::endl;


        std::cout << "\n=== Solving system ===" << std::endl;
        solver.solve(graph);


        std::cout << "\nAfter solving:" << std::endl;
        std::cout << "a = " << std::any_cast<double>(a->value) << std::endl;
        std::cout << "b = " << std::any_cast<double>(b->value) << " (should be 20)" << std::endl;
        std::cout << "c = " << std::any_cast<double>(c->value) << " (should be 25)" << std::endl;
        std::cout << "d = " << std::any_cast<double>(d->value) << std::endl;


        std::cout << "\n=== Changing a to 15 ===" << std::endl;
        a->value = 15.0;
        solver.solve(graph);

        std::cout << "\nAfter update:" << std::endl;
        std::cout << "a = " << std::any_cast<double>(a->value) << std::endl;
        std::cout << "b = " << std::any_cast<double>(b->value) << " (should be 30)" << std::endl;
        std::cout << "c = " << std::any_cast<double>(c->value) << " (should be 35)" << std::endl;
        std::cout << "d = " << std::any_cast<double>(d->value) << std::endl;


        std::cout << "\n=== Removing constraint b = a * 2 ===" << std::endl;
        for (auto& c : graph.getConstraints()) {
            if (c->getStrength() == 100) {
                solver.removeConstraint(graph, c.get());
                break;
            }
        }
        solver.solve(graph);

        std::cout << "\nAfter removal:" << std::endl;
        std::cout << "a = " << std::any_cast<double>(a->value) << std::endl;
        std::cout << "b = " << std::any_cast<double>(b->value) << " (stay value)" << std::endl;
        std::cout << "c = " << std::any_cast<double>(c->value) << " (should be b + d)" << std::endl;
        std::cout << "d = " << std::any_cast<double>(d->value) << std::endl;

        ConstraintGraph::GraphPrinter printer;
        printer.printGraph(graph);


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
