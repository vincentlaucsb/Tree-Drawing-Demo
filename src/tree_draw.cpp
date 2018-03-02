// This code contains routines for building and drawing trees

#include "tree.h"

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

TreeNode IncompleteBinaryTree::make_tree(int depth) {
    TreeNode tree;
    make_tree(tree, depth);
    return tree;
}

void IncompleteBinaryTree::make_tree(TreeNode& tree, int depth) {
    // Generate an incomplete binary tree recursively
    if (depth) {
        auto lchance = distribution(generator), rchance = distribution(generator);
        if (lchance > 0.2) tree.left = new TreeNode();
        if (rchance > 0.2) tree.right = new TreeNode();
        if (tree.left) this->make_tree(*(tree.left), depth - 1);
        if (tree.right) this->make_tree(*(tree.right), depth - 1);
    }
}

void draw_tree(SVG::Group& edges, SVG::Group& vertices, TreeNode& tree) {
    // Draw an SVG tree recursively
    vertices.add_child(SVG::Circle(tree.x, tree.y, 3 ));
    if (tree.left) {
        draw_tree(edges, vertices, *(tree.left));
        edges.add_child(SVG::Line(tree.x, tree.left->x, tree.y, tree.left->y));
    }

    if (tree.right) {
        draw_tree(edges, vertices, *(tree.right));
        edges.add_child(SVG::Line(tree.x, tree.right->x, tree.y, tree.right->y));
    }
}

SVG::SVG draw_binary_tree(const int depth, const DrawOpts& options) {
    // Draw a binary tree of depth n
    TreeNode tree_root;
    SVG::SVG svg_root;
    SVG::Group edges, vertices;
    edges.set_attr("stroke", options.edge_color)
         .set_attr("stroke-width", options.edge_width);

    binary_tree(&tree_root, depth);
    tree_root.calculate_xy();
    draw_tree(edges, vertices, tree_root);

    svg_root.add_child(edges, vertices);
    svg_root.set_bbox();
    return svg_root;
}