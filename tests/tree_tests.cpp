#define CATCH_CONFIG_MAIN
#include <vector>
#include "catch.hpp"
#include "tree.h"

TEST_CASE("right_contour() Test - Easy", "[rcontour_test]") {
    TreeNode root;
    root.add_left();
    root.add_right()->add_right();

    std::vector<TreeBase*> correct = { &root, root.right(), root.right()->right() };
    REQUIRE(root.right_contour() == correct);
}

TEST_CASE("right_contour() Test - Hard", "[rcontour_test2]") {
    TreeNode root;
    root.add_left();
    root.add_right();
    root.right()->add_right()->add_left()->add_right()->add_left();
    std::vector<TreeBase*> correct = { &root, root.right(), root.right()->right(), root.right()->right()->left(),
                                       root.right()->right()->left()->right(), root.right()->right()->left()->right()->left() };
    REQUIRE(root.right_contour() == correct);
}

TEST_CASE("right_contour() Test - Harder", "[rcontour_test3]") {
    TreeNode root;

    // Populate left side
    root.add_left()->add_left()->add_left()->add_left();

    // Populate right side
    root.add_right()->add_right()->add_right();

    std::vector<TreeBase*> correct = { &root, root.right(), root.right()->right(),
        root.right()->right()->right(), root.left()->left()->left()->left() };
    REQUIRE(root.right_contour() == correct);
}

TEST_CASE("Cumulative Displacement Test", "[cd_test]") {
    TreeNode root;
    root.displacement = 0;

    root.add_left()->displacement = -1;
    root.add_right()->displacement = 1;

    REQUIRE(root.distance_between(root.left(), root.right()) == 2);
}

TEST_CASE("Distance Between Test - Harder", "[cd_test_harder]") {
    TreeNode r1, r2;
    r1.displacement = 0;
    r1.add_left()->displacement = -1;
    r1.add_right()->displacement = 1;

    r2.displacement = 0;
    r2.add_left()->displacement = -1;
    r2.add_right()->displacement = 1;

    REQUIRE(r1.distance_between(&r1, &r2) == 4);
}

TEST_CASE("Distance Between Test - Harder 2", "[cd_test_harder2]") {
    TreeNode r1, r2;
    r1.displacement = 0;
    r1.add_left()->displacement = -1;
    r1.add_right()->displacement = 1;

    r2.displacement = 0;
    r2.add_left()->displacement = -1;
    r2.add_right()->displacement = 1;

    REQUIRE(r1.distance_between(&r1, &r2) == 4);
}