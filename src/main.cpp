#include "methods_helpers.hpp"
#include "property_model/builder.hpp"
#include "graph/utils.hpp"

using namespace NSConstraintGraph;

int absolute_w(int relate_width, int initial_width) {
    return initial_width * relate_width / 100;
}

int relative_w(int absoulute_width, int initial_width) {
    return (absoulute_width * 100) / initial_width;
}

int absolute_h(int relate_height, int initial_height) {
    return initial_height * relate_height / 100;
}

int relative_h(int absolute_height, int initial_height) {
    return (absolute_height * 100) / initial_height;
}

int preserve_relative_h(int relative_w) {
    return relative_w;
}

int preserve_relative_w(int relative_h) {
    return relative_h;
}

std::string output(int absolute_h, int absolute_w) {
    return std::to_string(absolute_h) + " " + std::to_string(absolute_w);
}

int main() {
    Builder pmb;

    pmb.addVariable(PropertyModel::Type::DataVariable, "initial_h", 1000);
    pmb.addVariable(PropertyModel::Type::DataVariable, "initial_w", 2000);

    pmb.addVariable(PropertyModel::Type::ValueVariable, "relative_h", 100);
    pmb.addVariable(PropertyModel::Type::ValueVariable, "absolute_h", 1000);
    pmb.addVariable(PropertyModel::Type::ValueVariable, "relative_w", 100);
    pmb.addVariable(PropertyModel::Type::ValueVariable, "absolute_w", 2000);

    pmb.addVariable(PropertyModel::Type::OutputVariable, "output", "1000 2000");

    pmb.addConstraint({"initial_w", "relative_w", "absolute_w"}, 1, true);
    pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&absolute_w), {"relative_w", "initial_w"}, "absolute_w");
    pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&relative_w), {"absolute_w", "initial_w"}, "relative_w");

    pmb.addConstraint({"initial_h", "relative_h", "absolute_h"}, 2, true);
    pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&absolute_h), {"relative_h", "initial_h"}, "absolute_h");
    pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&relative_h), {"absolute_h", "initial_h"}, "relative_h");

    // pmb.addConstraint({"relative_h", "relative_w"}, 3, false);
    // pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&preserve_relative_h), {"relative_w"}, "relative_h");
    // pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&preserve_relative_w), {"relative_h"}, "relative_w");

    pmb.addConstraint({"absolute_h", "absolute_w"}, 4, true);
    pmb.addMethod(NSMethodsHelpers::makeAnyFunction(&output), {"absolute_h", "absolute_w"}, "output");

    auto pm = pmb.extract();
    auto gp = GraphPrinter();
    std::cout << "----------------- Initial graph -----------------" << std::endl;
    gp.printGraph(pm->constraint_graph_);
    std::cout << std::endl << std::endl << std::endl << std::endl;

    pm->set("relative_h", 50);
    std::cout << "----------------- pm->set('relative_h', 50); -----------------" << std::endl;
    gp.printGraph(pm->constraint_graph_);
    std::cout << std::endl << std::endl << std::endl << std::endl;

    pm->set("absolute_w", 200);
    std::cout << "----------------- pm->set('absolute_w', 200); -----------------" << std::endl;
    gp.printGraph(pm->constraint_graph_);
    std::cout << std::endl << std::endl << std::endl << std::endl;

    pm->set("relative_w", 20);
    std::cout << "----------------- pm->set('relative_w', 20); -----------------" << std::endl;
    gp.printGraph(pm->constraint_graph_);
    std::cout << std::endl << std::endl << std::endl << std::endl;

    // pm->rough_enable(2);
    // gp.printGraph(pm->constraint_graph_);
    return 0;
}
