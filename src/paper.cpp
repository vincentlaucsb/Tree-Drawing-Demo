// Generate figures needed to write the paper

#include "tree.h"
#include "nary_tree.h"
#include <fstream>

SVG::SVG draw_fig2(const DrawOpts& options);

SVG::SVG draw_fig2(const DrawOpts& options) {
    /** Replicate figure 2 from RT81 */
    TreeNode root;
    SVG::SVG fig2;

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

    // Calculate coordinates    
    root.calculate_xy(0, 0, options);

    // Create SVG
    draw_tree(fig2, root, options);
    fig2.style("text").set_attr("font-family", "sans-serif");
    fig2.autoscale();
    return fig2;
}

int main(int argc, char** argv) {
    std::ofstream fig2("figure2.svg"),
        fig2_contours("figure2_contours.svg"),
        fig4("figure4.svg");

    DrawOpts options = DEFAULT_DRAWING_OPTIONS;
    options.x_sep = 20;
    options.y_sep = 30;
    options.node_size = 5;

    /*
     * Replicate Figure 2 and illustrate left/right contours
     */

    SVG::SVG fig2a = draw_fig2(options),
        fig2_temp1 = draw_fig2(options),
        fig2_temp2 = draw_fig2(options);

    // Highlight left & right contours respectively
    left_contour(fig2_temp1);
    right_contour(fig2_temp2);
    SVG::SVG fig2_contours_drawing = merge(fig2_temp1, fig2_temp2);

    fig2 << std::string(fig2a);
    fig2_contours << std::string(fig2_contours_drawing);

    /*
     * Replicate Figure 4
     */

    TreeNode left;
    left.add_right();
    left.add_left()->add_right()->add_right()->add_right()->add_right();

    TreeNode right;
    right.add_left();
    right.add_right()->add_left()->add_left()->add_left()->add_left();

    SVG::SVG tree1, tree2;
    left.calculate_xy(0, 0, options);
    right.calculate_xy(0, 0, options);

    /*
     * Complete Binary Trees
     */
    for (int i = 0; i <= 5; i++) {
        TreeNode binary_root;
        SVG::SVG binary_svg;
        binary_tree(&binary_root, i);
        binary_root.calculate_xy(0, 0, options);
        label_tree_disp(&binary_root);

        // Output
        std::ofstream binary_out("binary_tree_" + std::to_string(i) + ".svg");
        draw_tree(binary_svg, binary_root, options);
        binary_svg.autoscale({ 20, 20, 20, 20 });
        binary_out << std::string(binary_svg);
    }

    /*
     * Complete Ternary Trees
     */
    NaryTreeNode ternary_root;
    SVG::SVG ternary_svg;
    nary_tree(&ternary_root, 3, 5);
    ternary_root.calculate_xy(0, 0, options);
    label_tree_disp(&ternary_root);
    draw_tree(ternary_svg, ternary_root, options);
    std::ofstream ternary_out("ternary_tree.svg");
    ternary_out << std::string(ternary_svg);

    // Draw SVG
    draw_tree(tree1, left, options);
    draw_tree(tree2, right, options);

    auto tree3 = merge(tree1, tree2);
    fig4 << std::string(tree3);
    return 0;
}