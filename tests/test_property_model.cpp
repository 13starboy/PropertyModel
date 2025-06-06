#include <gtest/gtest.h>

#include <pmodel/graph/utils.hpp>
#include <pmodel/property_model/builder.hpp>
#include <pmodel/utils/methods_helpers.hpp>

using namespace NSPropertyModel;

/*========================================
||                                      ||
||         Modal Window TEST            ||
||                                      ||
========================================*/

int absolute_w(int relate_width, int initial_width) {
    return initial_width * relate_width / 100;
}

int relative_w(int absoulute_width, int initial_width) {
    return (absoulute_width * 100) / initial_width;
}

int absolute_h(int relative_height, int initial_height) {
    return initial_height * relative_height / 100;
}

int relative_h(int absolute_height, int initial_height) {
    return (absolute_height * 100) / initial_height;
}

std::string absolute_size_summary(int absolute_h, int absolute_w) {
    return std::to_string(absolute_w) + " x " + std::to_string(absolute_h);
}

std::string relative_size_summary(int relative_h, int relative_w) {
    return std::to_string(relative_w) + " x " + std::to_string(relative_h);
}

TEST(PropertyModelTest, ModalWindow) {
    Builder pmb;

    pmb.addVariable(PropertyModel::Type::DataVariable, "initial_w", 2000);
    pmb.addVariable(PropertyModel::Type::DataVariable, "initial_h", 1000);

    pmb.addVariable(PropertyModel::Type::ValueVariable, "relative_w", 100);
    pmb.addVariable(PropertyModel::Type::ValueVariable, "relative_h", 100);
    pmb.addVariable(PropertyModel::Type::ValueVariable, "absolute_w", 2000);
    pmb.addVariable(PropertyModel::Type::ValueVariable, "absolute_h", 1000);

    pmb.addVariable(PropertyModel::Type::OutputVariable, "absolute_size_summary", std::string("2000 x 1000"));
    pmb.addVariable(PropertyModel::Type::OutputVariable, "relative_size_summary", std::string("100 x 100"));

    pmb.addConstraint({"initial_w", "relative_w", "absolute_w"}, 1, true);
    pmb.addMethod(NSPropertyModelHelpers::makeAnyFunction(&absolute_w), {"relative_w", "initial_w"}, "absolute_w");
    pmb.addMethod(NSPropertyModelHelpers::makeAnyFunction(&relative_w), {"absolute_w", "initial_w"}, "relative_w");

    pmb.addConstraint({"initial_h", "relative_h", "absolute_h"}, 2, true);
    pmb.addMethod(NSPropertyModelHelpers::makeAnyFunction(&absolute_h), {"relative_h", "initial_h"}, "absolute_h");
    pmb.addMethod(NSPropertyModelHelpers::makeAnyFunction(&relative_h), {"absolute_h", "initial_h"}, "relative_h");

    pmb.addConstraint({"absolute_h", "absolute_w"}, 3, true);
    pmb.addMethod(
        NSPropertyModelHelpers::makeAnyFunction(&absolute_size_summary), {"absolute_h", "absolute_w"},
        "absolute_size_summary"
    );

    pmb.addConstraint({"relative_h", "relative_w"}, 4, true);
    pmb.addMethod(
        NSPropertyModelHelpers::makeAnyFunction(&relative_size_summary), {"relative_h", "relative_w"},
        "relative_size_summary"
    );

    auto pm = pmb.extract();
    // ------------------------- Initial State -------------------------
    EXPECT_EQ(std::any_cast<int>(pm->get("initial_w")), 2000);
    EXPECT_EQ(std::any_cast<int>(pm->get("initial_h")), 1000);

    EXPECT_EQ(std::any_cast<int>(pm->get("relative_w")), 100);
    EXPECT_EQ(std::any_cast<int>(pm->get("relative_h")), 100);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_w")), 2000);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_h")), 1000);

    EXPECT_EQ(std::any_cast<std::string>(pm->get("absolute_size_summary")), "2000 x 1000");
    EXPECT_EQ(std::any_cast<std::string>(pm->get("relative_size_summary")), "100 x 100");

    // ------------------------- Relative_h = 50 -------------------------
    pm->set("relative_h", 50);

    EXPECT_EQ(std::any_cast<int>(pm->get("initial_w")), 2000);
    EXPECT_EQ(std::any_cast<int>(pm->get("initial_h")), 1000);

    EXPECT_EQ(std::any_cast<int>(pm->get("relative_w")), 100);
    EXPECT_EQ(std::any_cast<int>(pm->get("relative_h")), 50);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_w")), 2000);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_h")), 500);

    EXPECT_EQ(std::any_cast<std::string>(pm->get("absolute_size_summary")), "2000 x 500");
    EXPECT_EQ(std::any_cast<std::string>(pm->get("relative_size_summary")), "100 x 50");

    // ------------------------- Absolute_w = 200 -------------------------
    pm->set("absolute_w", 200);

    EXPECT_EQ(std::any_cast<int>(pm->get("initial_w")), 2000);
    EXPECT_EQ(std::any_cast<int>(pm->get("initial_h")), 1000);

    EXPECT_EQ(std::any_cast<int>(pm->get("relative_w")), 10);
    EXPECT_EQ(std::any_cast<int>(pm->get("relative_h")), 50);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_w")), 200);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_h")), 500);

    EXPECT_EQ(std::any_cast<std::string>(pm->get("absolute_size_summary")), "200 x 500");
    EXPECT_EQ(std::any_cast<std::string>(pm->get("relative_size_summary")), "10 x 50");

    // ------------------------- Relative_w = 20 -------------------------
    pm->set("relative_w", 20);
    EXPECT_EQ(std::any_cast<int>(pm->get("initial_w")), 2000);
    EXPECT_EQ(std::any_cast<int>(pm->get("initial_h")), 1000);

    EXPECT_EQ(std::any_cast<int>(pm->get("relative_w")), 20);
    EXPECT_EQ(std::any_cast<int>(pm->get("relative_h")), 50);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_w")), 400);
    EXPECT_EQ(std::any_cast<int>(pm->get("absolute_h")), 500);

    EXPECT_EQ(std::any_cast<std::string>(pm->get("absolute_size_summary")), "400 x 500");
    EXPECT_EQ(std::any_cast<std::string>(pm->get("relative_size_summary")), "20 x 50");
}

/*========================================
||                                      ||
||      Booking with Cycle TEST         ||
||                                      ||
========================================*/

int start_day_with_cycle(int start_day, int end_day) {
    return start_day;
}

TEST(PropertyModelTest, BookingWithCycleTest) {
    Builder pmb;

    pmb.addVariable(PropertyModel::Type::DataVariable, "start_day", 1);
    pmb.addVariable(PropertyModel::Type::DataVariable, "end_day", 15);

    pmb.addConstraint({"start_day", "end_day"}, 1, true);
    pmb.addMethod(
        NSPropertyModelHelpers::makeAnyFunction(&start_day_with_cycle), {"start_day", "end_day"}, "start_day"
    );
    EXPECT_THROW(
        {
            try {
                auto pm = pmb.extract();
            } catch (const std::logic_error& e) {
                EXPECT_STREQ("Cycle found", e.what());
                throw;
            }
        },
        std::logic_error
    );
}

/*========================================
||                                      ||
||            Booking TEST              ||
||                                      ||
========================================*/

int vacation_length(int start_day, int end_day) {
    return end_day - start_day;
}

int start_day(int vacation_length, int end_day) {
    return end_day - vacation_length;
}

int end_day(int start_day, int vacation_length) {
    return start_day + vacation_length;
}

TEST(PropertyModelTest, BookingTest) {
    Builder pmb;

    pmb.addVariable(PropertyModel::Type::DataVariable, "start_day", 1);
    pmb.addVariable(PropertyModel::Type::DataVariable, "end_day", 15);
    pmb.addVariable(PropertyModel::Type::DataVariable, "vacation_length", 14);

    pmb.addConstraint({"start_day", "end_day", "vacation_length"}, 1, true);
    pmb.addMethod(
        NSPropertyModelHelpers::makeAnyFunction(&vacation_length), {"start_day", "end_day"}, "vacation_length"
    );
    pmb.addMethod(NSPropertyModelHelpers::makeAnyFunction(&end_day), {"start_day", "vacation_length"}, "end_day");

    auto pm = pmb.extract();

    // ------------------------- Initial State -------------------------
    EXPECT_EQ(std::any_cast<int>(pm->get("start_day")), 1);
    EXPECT_EQ(std::any_cast<int>(pm->get("end_day")), 15);
    EXPECT_EQ(std::any_cast<int>(pm->get("vacation_length")), 14);

    // ------------------------- vacation_length = 30 -------------------------
    pm->set("vacation_length", 30);
    EXPECT_EQ(std::any_cast<int>(pm->get("start_day")), 1);
    EXPECT_EQ(std::any_cast<int>(pm->get("end_day")), 31);
    EXPECT_EQ(std::any_cast<int>(pm->get("vacation_length")), 30);

    // ------------------------- start_day = 10 -------------------------
    pm->set("start_day", 10);
    EXPECT_EQ(std::any_cast<int>(pm->get("start_day")), 10);
    EXPECT_EQ(std::any_cast<int>(pm->get("end_day")), 40);
    EXPECT_EQ(std::any_cast<int>(pm->get("vacation_length")), 30);

    // ------------------------- end_day = 20 -------------------------
    pm->set("start_day", 20);
    EXPECT_EQ(std::any_cast<int>(pm->get("start_day")), 20);
    EXPECT_EQ(std::any_cast<int>(pm->get("end_day")), 50);
    EXPECT_EQ(std::any_cast<int>(pm->get("vacation_length")), 30);
}
