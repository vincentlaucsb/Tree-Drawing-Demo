// This code contains routines for building and drawing trees

#include "tree.h"
#include "nary_tree.h"

void binary_tree(TreeNode* tree, int depth) {
    // A function that creates a complete binary tree recursively
    TreeNode *left, *right;

    if (depth) {
        left = tree->add_left();
        right = tree->add_right();
        binary_tree(left, depth - 1);
        binary_tree(right, depth - 1);
    }
}

NaryTreeNode nary_tree(int n, int height) {
    // Create a complete nary tree of specified height
    NaryTreeNode root;
    nary_tree(&root, n, height);
    return root;
}

void nary_tree(NaryTreeNode* tree, int n, int depth) {
    // A function that creates a complete nary tree recursively
    NaryTreeNode* child;

    if (depth) {
        for (int i = 0; i < n; i++) {
            child = tree->add_child();
            nary_tree(child, n, depth - 1);
        }
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
        if (lchance > 0.2) tree.add_left();
        if (rchance > 0.2) tree.add_right();
        if (tree.left()) this->make_tree(*(tree.left()), depth - 1);
        if (tree.right()) this->make_tree(*(tree.right()), depth - 1);
    }
}

void draw_tree(SVG::Group* edges, SVG::Group* vertices, TreeNode& tree, const DrawOpts& options) {
    // Draw an SVG tree recursively
    vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);
    if (tree.left()) {
        draw_tree(edges, vertices, *(tree.left()), options);
        edges->add_child<SVG::Line>(tree.x, tree.left()->x, tree.y, tree.left()->y);
    }

    if (tree.right()) {
        draw_tree(edges, vertices, *(tree.right()), options);
        edges->add_child<SVG::Line>(tree.x, tree.right()->x, tree.y, tree.right()->y);
    }

    edges->set_attr("stroke", options.edge_color)
            .set_attr("stroke-width", options.edge_width);
}

void draw_tree(SVG::Group* edges, SVG::Group* vertices, NaryTreeNode& tree, const DrawOpts& options) {
    // Draw an SVG tree recursively
    vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);
    for (auto& child: tree.children) {
        draw_tree(edges, vertices, *(child), options);
        edges->add_child<SVG::Line>(tree.x, child->x, tree.y, child->y);
    }
}

SVG::SVG draw_binary_tree(const int depth, const DrawOpts& options) {
    // Draw a binary tree of depth n
    TreeNode tree_root;
    SVG::SVG svg_root;
    auto edges = svg_root.add_child<SVG::Group>(),
        vertices = svg_root.add_child<SVG::Group>();
    edges->set_attr("stroke", options.edge_color)
         .set_attr("stroke-width", options.edge_width);

    binary_tree(&tree_root, depth);
    tree_root.calculate_xy(0, 0, options);
    draw_tree(edges, vertices, tree_root, options);
    svg_root.autoscale();
    return svg_root;
}

SVG::SVG draw_nary_tree(const int n, const int height, const DrawOpts& options) {
    // Draw a complete nary tree of specified height
    NaryTreeNode tree = nary_tree(n, height);
    SVG::SVG svg_root;
    auto edges = svg_root.add_child<SVG::Group>(),
        vertices = svg_root.add_child<SVG::Group>();

    edges->set_attr("stroke", options.edge_color)
            .set_attr("stroke-width", options.edge_width);

    tree.calculate_xy(0, 0, options);
    draw_tree(edges, vertices, tree, options);
    svg_root.autoscale();
    return svg_root;
}