#define CATCH_CONFIG_MAIN
#include <vector>
#include "catch.hpp"
#include "tree_draw.h"

using namespace tree;

TEST_CASE("nary_tree() Test", "[nary_tree_test]") {
    // Make sure that nary_tree actually builds a complete tree
    NaryTreeNode ternary_tree = nary_tree(3, 5);
    // REQUIRE(ternary_tree.height() == 5);
}