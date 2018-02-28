#define CATCH_CONFIG_MAIN
#include <vector>
#include "catch.hpp"
#include "tree.h"

TEST_CASE("right_contour() Test - Easy", "[rcontour_test]") {
    TreeNode root;
    root.left = new TreeNode;
    root.right = new TreeNode;
    root.right->right = new TreeNode;

    std::vector<TreeNode*> correct = { &root, root.right, root.right->right };
    REQUIRE(root.right_contour() == correct);
}

TEST_CASE("right_contour() Test - Hard", "[rcontour_test2]") {
    TreeNode root;
    root.left = new TreeNode;
    root.right = new TreeNode;
    root.right->right = new TreeNode;
    root.right->right->left = new TreeNode;
    root.right->right->left->right = new TreeNode;
    root.right->right->left->right->left = new TreeNode;

    std::vector<TreeNode*> correct = { &root, root.right, root.right->right, root.right->right->left,
                                       root.right->right->left->right, root.right->right->left->right->left };
    REQUIRE(root.right_contour() == correct);
}

TEST_CASE("right_contour() Test - Harder", "[rcontour_test3]") {
    TreeNode root;

    // Populate left side
    root.left = new TreeNode;
    root.left->left = new TreeNode;
    root.left->left->left = new TreeNode;
    root.left->left->left->left = new TreeNode;

    // Populate right side
    root.right = new TreeNode;
    root.right->right = new TreeNode;
    root.right->right->right = new TreeNode;

    std::vector<TreeNode*> correct = { &root, root.right, root.right->right,
        root.right->right->right, root.left->left->left->left };
    REQUIRE(root.right_contour() == correct);
}

TEST_CASE("Cumulative Displacement Test", "[cd_test]") {
    TreeNode root;
    root.displacement = 0;

    root.left = new TreeNode;
    root.left->displacement = -1;

    root.right = new TreeNode;
    root.right->displacement = 1;

    REQUIRE(root.distance_between(root.left, root.right) == 2);
}

TEST_CASE("Distance Between Test - Harder", "[cd_test_harder]") {
    TreeNode r1, r2;
    r1.displacement = 0;
    r1.left = new TreeNode;
    r1.left->displacement = -1;
    r1.right = new TreeNode;
    r1.right->displacement = 1;

    r2.displacement = 0;
    r2.left = new TreeNode;
    r2.left->displacement = -1;
    r2.right = new TreeNode;
    r2.right->displacement = 1;

    REQUIRE(r1.distance_between(&r1, &r2) == 4);
}

TEST_CASE("Distance Between Test - Harder 2", "[cd_test_harder2]") {
    TreeNode r1, r2;
    r1.displacement = 0;
    r1.left = new TreeNode;
    r1.left->displacement = -1;
    r1.right = new TreeNode;
    r1.right->displacement = 1;

    r2.displacement = 0;
    r2.left = new TreeNode;
    r2.left->displacement = -1;
    r2.right = new TreeNode;
    r2.right->displacement = 1;

    REQUIRE(r1.distance_between(&r1, &r2) == 4);
}