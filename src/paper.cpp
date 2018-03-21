// Generate figures needed to write the paper

#include "tree.h"
#include "nary_tree.h"
#include <fstream>

int main(int argc, char** argv) {
    std::ofstream fig2("figure2.svg"), fig4("figure4.svg");

    // Replicate Figure 2
    TreeNode root;

    // Left subtree
    auto d1_left = root.add_left();
    auto d2_left = d1_left->add_left();
    d1_left->add_right();
    auto d3_left = d2_left->add_left();
    d2_left->add_right();
    auto d4_left = d3_left->add_left();
    d3_left->add_right();
    d4_left->add_left();
    auto d5_right = d4_left->add_right();
    d5_right->add_left();
    auto d6_right = d5_right->add_right();
    auto d7_left = d6_right->add_right();
    d7_left->add_left();
    d7_left->add_right();

    // Right subtree
    auto d1_right = root.add_right();
    auto d2_right = d1_right->add_right();
    d1_right->add_left();
    auto d3_right = d2_right->add_right();
    d2_right->add_left();
    auto d4_right = d3_right->add_right();
    d3_right->add_left();
    d4_right->add_right();
    auto d5_left = d4_right->add_left();
    d5_left->add_right();
    auto d6_left = d5_left->add_left();
    auto d7_right = d6_left->add_left();
    d7_right->add_left();
    d7_right->add_right();

    // Replicate Figure 4
    TreeNode left;
    left.add_right();
    left.add_left()->add_right()->add_right()->add_right()->add_right();

    TreeNode right;
    right.add_left();
    right.add_right()->add_left()->add_left()->add_left()->add_left();

    // Produce SVG figures
    DrawOpts options = DEFAULT_DRAWING_OPTIONS;
    options.x_sep = 20;
    options.y_sep = 30;
    options.node_size = 5;

    SVG::SVG fig2_tree, tree1, tree2;
    auto *fig2_e = fig2_tree.add_child<SVG::Group>(),
        *fig2_v = fig2_tree.add_child<SVG::Group>(),
        *e1 = tree1.add_child<SVG::Group>(),
        *v1 = tree1.add_child<SVG::Group>(),
        *e2 = tree2.add_child<SVG::Group>(),
        *v2 = tree2.add_child<SVG::Group>();

    root.calculate_xy(0, 0, options);
    left.calculate_xy(0, 0, options);
    right.calculate_xy(0, 0, options);
    draw_tree(fig2_e, fig2_v, root, options);
    draw_tree(e1, v1, left, options);
    draw_tree(e2, v2, right, options);
    fig2_tree.autoscale();
    tree1.merge(tree2);
    fig2 << fig2_tree.to_string();
    fig4 << tree1.to_string();

    return 0;
}