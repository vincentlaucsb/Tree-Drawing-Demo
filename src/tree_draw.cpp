// This code contains routines for building and drawing trees

#include "tree.h"
#include "nary_tree.h"
#include "tree_draw.h"

void update_map_right(std::map<double, SVG::Circle*>& contour, SVG::Circle* node);
void update_map_left(std::map<double, SVG::Circle*>& contour, SVG::Circle* node);
void highlight_contour(SVG::SVG& root, std::function<void(std::map<double, SVG::Circle*>&, SVG::Circle*)> func);

void update_map_left(std::map<double, SVG::Circle*>& contour, SVG::Circle* node) {
    if ((contour.find(node->y()) == contour.end()) || (node->x() < contour[node->y()]->x()))
        contour[node->y()] = node;
}

void update_map_right(std::map<double, SVG::Circle*>& contour, SVG::Circle* node) {
    if ((contour.find(node->y()) == contour.end()) || (node->x() > contour[node->y()]->x()))
        contour[node->y()] = node;
};

void highlight_contour(SVG::SVG& root,
    std::function<void(std::map<double, SVG::Circle*>&, SVG::Circle*)> func) {
    /** Given a drawn tree, highlight the left or right contour */
    auto nodes = root.get_children<SVG::Circle>();

    // For every layer in the drawing (y-position), find left/right-most vertex
    std::map<double, SVG::Circle*> contour;
    for (auto& node : nodes) func(contour, node);

    // Create boundary for highlighted nodes
    for (auto& circ : contour) circ.second->set_attr("class", "contour");
    root.style("circle.contour").set_attr("fill", "red");
}

void left_contour(SVG::SVG& root) {
    /** Given a drawn tree, highlight the left contour */
    auto func = &update_map_left;
    highlight_contour(root, func);
}

void right_contour(SVG::SVG& root) {
    /** Given a drawn tree, highlight the right contour */
    auto func = &update_map_right;
    highlight_contour(root, func);
}

void binary_tree(TreeNode* tree, int depth) {
    /** A function that creates a complete binary tree recursively */
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

TreeDraw::TreeDraw(SVG::SVG* _tree_svg, const DrawOpts& _options) :
    tree_svg(_tree_svg), options(_options) {
    tree_svg->style("circle.left_node").set_attr("text-anchor", "end");
    tree_svg->style("circle.leaf").set_attr("text-anchor", "middle");
    edges = tree_svg->add_child<SVG::Group>();
    vertices = tree_svg->add_child<SVG::Group>();
    vertices->set_attr("class", "vertices");
    edges->set_attr("class", "edges")
        .set_attr("stroke", options.edge_color)
        .set_attr("stroke-width", options.edge_width);
}

void TreeDraw::draw_tree(TreeNode& tree, bool disp_label) {
    /** Draw a SVG tree recursively
     *  disp_label: Give a tree's nodes a label equal to its displacement
     */
    auto left = tree.left(), right = tree.right();
    vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);

    if (left) {
        if (disp_label) left->label = SVG::to_string(left->displacement);
        this->draw_tree(*left, disp_label);
        edges->add_child<SVG::Line>(tree.x, left->x, tree.y, left->y);

        // Add text labels
        if (!left->label.empty()) {
            double x_pos = left->x - 20;
            if (left->is_leaf())
                vertices->add_child<SVG::Text>(x_pos + 5, left->y + 20, left->label)
                    ->set_attr("class", "leaf");
            else
                vertices->add_child<SVG::Text>(x_pos, left->y - 10, left->label)
                    ->set_attr("class", "left_node");
                
        }
    }

    if (right) {
        if (disp_label) right->label = SVG::to_string(right->displacement);
        this->draw_tree(*right, disp_label);
        edges->add_child<SVG::Line>(tree.x, right->x, tree.y, right->y);

        // Add text labels
        if (!right->label.empty()) {
            if (right->is_leaf())
                vertices->add_child<SVG::Text>(right->x - 5, right->y + 20, right->label)
                    ->set_attr("class", "leaf");
            else
                vertices->add_child<SVG::Text>(right->x, right->y - 10, right->label);
        }
    }
}

void TreeDraw::draw_tree(NaryTreeNode& tree, bool disp_label) {
    /** Draw an SVG tree recursively */
    vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);

    // Add text labels
    if (!tree.label.empty())
        vertices->add_child<SVG::Text>(tree.x, tree.y, tree.label);

    for (auto& child : tree.children) {
        if (disp_label) child->label = SVG::to_string(child->displacement);
        this->draw_tree(*child, disp_label);
        edges->add_child<SVG::Line>(tree.x, child->x, tree.y, child->y);
    }
}

SVG::SVG draw_binary_tree(const int height, const DrawOpts& options, bool disp_label) {
    /** Draw a binary tree of height n */
    TreeNode root;
    binary_tree(&root, height);
    root.calculate_xy(0, 0, options);
    return draw_tree(root, options, disp_label);
}

SVG::SVG draw_nary_tree(const int n, const int height, const DrawOpts& options, bool disp_label) {
    /** Draw a complete nary tree of specified height */
    NaryTreeNode root = nary_tree(n, height);
    root.calculate_xy(0, 0, options);
    return draw_tree(root, options, disp_label);
}