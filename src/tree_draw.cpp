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

void draw_tree(SVG::SVG& svg, TreeNode& tree, const DrawOpts& options) {
    // Draw an SVG tree recursively
    SVG::Element *vertices, *edges;
    auto t1 = svg.get_elements_by_class("vertices"),
        t2 = svg.get_elements_by_class("edges");

    if (t1.empty()) {
        vertices = svg.add_child<SVG::Group>();
        vertices->set_attr("class", "vertices");
    }
    else vertices = t1[0];

    if (t2.empty()) {
        edges = svg.add_child<SVG::Group>();
        edges->set_attr("class", "edges");
    }
    else edges = t2[0];

    vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);
    if (tree.left()) {
        draw_tree(svg, *(tree.left()), options);
        edges->add_child<SVG::Line>(tree.x, tree.left()->x, tree.y, tree.left()->y);

        // Add text labels
        if (!tree.left()->label.empty())
            vertices->add_child<SVG::Text>(tree.left()->x, tree.left()->y, tree.left()->label);
    }

    if (tree.right()) {
        draw_tree(svg, *(tree.right()), options);
        edges->add_child<SVG::Line>(tree.x, tree.right()->x, tree.y, tree.right()->y);

        // Add text labels
        if (!tree.right()->label.empty())
            vertices->add_child<SVG::Text>(tree.right()->x, tree.right()->y, tree.right()->label);
    }

    edges->set_attr("stroke", options.edge_color)
            .set_attr("stroke-width", options.edge_width);
}

void label_tree_disp(TreeNode* tree) {
    // Give a tree's nodes a label equal to its displacement (recursively)
    tree->label = SVG::double_to_string(tree->displacement);
    if (tree->left()) label_tree_disp(tree->left());
    if (tree->right()) label_tree_disp(tree->right());
}

void label_tree_disp(NaryTreeNode* tree) {
    // Give a tree, give nodes a label equal to their displacement (recursively)
    tree->label = SVG::double_to_string(tree->displacement);
    for (auto& child: tree->children)
        label_tree_disp(child.get());
}

void draw_tree(SVG::SVG& svg, NaryTreeNode& tree, const DrawOpts& options) {
    // Draw an SVG tree recursively
    SVG::Element *vertices, *edges;
    auto t1 = svg.get_elements_by_class("vertices"),
        t2 = svg.get_elements_by_class("edges");

    if (t1.empty()) {
        vertices = svg.add_child<SVG::Group>();
        vertices->set_attr("class", "vertices");
    }
    else vertices = t1[0];

    if (t2.empty()) {
        edges = svg.add_child<SVG::Group>();
        edges->set_attr("class", "edges");
    }
    else edges = t2[0];

    vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);

    // Add text labels
    if (!tree.label.empty())
        vertices->add_child<SVG::Text>(tree.x, tree.y, tree.label);

    for (auto& child: tree.children) {
        draw_tree(svg, *child, options);
        edges->add_child<SVG::Line>(tree.x, child->x, tree.y, child->y);
    }
}

SVG::SVG draw_binary_tree(const int depth, const DrawOpts& options) {
    // Draw a binary tree of depth n
    TreeNode tree_root;
    SVG::SVG svg_root;
    auto edges = svg_root.add_child<SVG::Group>("edges"),
        vertices = svg_root.add_child<SVG::Group>("vertices");
    edges->set_attr("stroke", options.edge_color)
         .set_attr("stroke-width", options.edge_width);

    binary_tree(&tree_root, depth);
    tree_root.calculate_xy(0, 0, options);

    // Add displacement labels
    label_tree_disp(&tree_root);

    // Draw SVG
    draw_tree(svg_root, tree_root, options);
    svg_root.autoscale();

    return svg_root;
}

SVG::SVG draw_nary_tree(const int n, const int height, const DrawOpts& options) {
    // Draw a complete nary tree of specified height
    NaryTreeNode tree = nary_tree(n, height);
    SVG::SVG svg_root;
    auto edges = svg_root.add_child<SVG::Group>("edges"),
        vertices = svg_root.add_child<SVG::Group>("vertices");

    edges->set_attr("stroke", options.edge_color)
            .set_attr("stroke-width", options.edge_width);

    tree.calculate_xy(0, 0, options);

    // Add displacement labels
    label_tree_disp(&tree);

    // Draw SVG
    draw_tree(svg_root, tree, options);
    svg_root.autoscale();
    return svg_root;
}