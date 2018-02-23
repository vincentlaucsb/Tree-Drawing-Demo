// Tree drawing example of a binary tree with uniform width and height

#include "svg/src/svg.hpp"
#include "tree.h"
#include <fstream>

void binary_tree(TreeNode* tree, int depth) {
    // A function that creates a complete binary tree recursively
    TreeNode *left, *right;

    if (depth) {
        left = tree->left = new TreeNode();
        right = tree->right = new TreeNode();
        binary_tree(left, depth - 1);
        binary_tree(right, depth - 1);
    }
}

void draw_tree(SVG::Group& edges, SVG::Group& vertices, TreeNode& tree) {
    // Draw an SVG tree recursively
    vertices.add_child(SVG::Circle(tree.x, tree.y, 25));
    if (tree.left) {
        draw_tree(edges, vertices, *(tree.left));
        edges.add_child(SVG::Line(tree.x, tree.left->x, tree.y, tree.left->y));
    }

    if (tree.right) {
        draw_tree(edges, vertices, *(tree.right));
        edges.add_child(SVG::Line(tree.x, tree.right->x, tree.y, tree.right->y));
    }
}

int main() {
    // Create a binary tree of depth 2
    TreeNode t1, t2;
    binary_tree(&t1, 2);
    binary_tree(&t2, 5);
    t1.calculate_xy();
    t2.calculate_xy();

    std::cout << t1.displacement << std::endl;
    std::cout << t1.left->displacement << " - " << t1.right->displacement << std::endl;
    std::cout << t1.left->left->displacement << ", " << t1.left->right->displacement <<
        " - " << t1.right->left->displacement << ", " << t1.right->right->displacement << std::endl << std::endl;

    std::cout << t2.displacement << std::endl;
    std::cout << t2.left->displacement << " - " << t2.right->displacement << std::endl;
    std::cout << t2.left->left->displacement << ", " << t2.left->right->displacement <<
        " - " << t2.right->left->displacement << ", " << t2.right->right->displacement << std::endl;

    // SVG
    SVG::SVG root;
    SVG::Group edges, nodes;
    edges.set_attr("stroke", "black")
        .set_attr("stroke-width", "1px");

    std::ofstream outfile("tree.svg");
    draw_tree(edges, nodes, t2);
    root.add_child(edges, nodes);
    outfile << root.to_string();

    return 0;
}