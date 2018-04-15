// This code contains routines for building and drawing trees

#include "tree.h"
#include "nary_tree.h"
#include "tree_draw.h"

namespace tree {
    namespace helpers {
        void binary_tree(TreeNode* tree, int depth) {
            /** A helper function that creates a complete binary tree recursively */
            TreeNode *left, *right;

            if (depth) {
                left = tree->add_left();
                right = tree->add_right();
                binary_tree(left, depth - 1);
                binary_tree(right, depth - 1);
            }
        }

        void nary_tree(NaryTreeNode* tree, int n, int depth) {
            /** A helper function that creates a complete nary tree recursively */
            NaryTreeNode* child;

            if (depth) {
                for (int i = 0; i < n; i++) {
                    child = tree->add_child();
                    nary_tree(child, n, depth - 1);
                }
            }
        }


        TreeDraw::TreeDraw(SVG::SVG* _tree_svg, const DrawOpts& _options) :
            tree_svg(_tree_svg), options(_options) {
            tree_svg->style("circle").set_attr("opacity", 0.5);
            tree_svg->style("circle.left_node").set_attr("text-anchor", "end");
            tree_svg->style("circle.leaf").set_attr("text-anchor", "middle");
            tree_svg->style("line.thread").set_attr("stroke-dasharray", "5,5");
            edges = tree_svg->add_child<SVG::Group>();
            vertices = tree_svg->add_child<SVG::Group>();
            vertices->set_attr("class", "vertices");
            edges->set_attr("class", "edges")
                .set_attr("stroke", options.edge_color)
                .set_attr("stroke-width", options.edge_width);
        }

        void TreeDraw::draw_tree(TreeBase& tree, bool disp_label) {
            /** Draw a SVG tree recursively
            *  disp_label: Give a tree's nodes a label equal to its displacement
            */
            vertices->add_child<SVG::Circle>(tree.x, tree.y, options.node_size);

            auto children = tree.get_children();
            for (size_t i = 0; i < children.size(); i++) {
                auto& child = children[i];
                bool is_left = (i == 0), is_right = ((i + 1) == children.size());

                if (disp_label) {
                    child->label = SVG::to_string(child->displacement);
                    if (child->thread_loffset || child->thread_roffset) child->label += " (" + SVG::to_string(child->thread_loffset) + ","
                        + SVG::to_string(child->thread_roffset) + ")";
                }
                
                auto edge = edges->add_child<SVG::Line>(tree.x, child->x, tree.y, child->y);
                if (tree.thread == child) edge->set_attr("class", "thread");
                else this->draw_tree(*child, disp_label); // Recursion

                // Add text labels
                if (!child->label.empty()) {
                    if (is_left)
                        vertices->add_child<SVG::Text>(child->x, child->y - 10, child->label)
                        ->set_attr("class", "left_node");
                    else if (is_right)
                        vertices->add_child<SVG::Text>(child->x, child->y - 10, child->label);
                    else
                        vertices->add_child<SVG::Text>(child->x, child->y - 10, child->label);
                }
            }
        }
    }

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

    TreeNode binary_tree(int height) {
        // Create a complete binary tree of specified height
        TreeNode root;
        helpers::binary_tree(&root, height);
        return root;
    }

    NaryTreeNode nary_tree(int n, int height) {
        // Create a complete nary tree of specified height
        NaryTreeNode root;
        helpers::nary_tree(&root, n, height);
        return root;
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

    SVG::SVG draw_tree(TreeBase& tree, const DrawOpts& options, bool disp_label) {
        /** Create an SVG drawing of the given binary or nary tree */
        SVG::SVG drawing;
        helpers::TreeDraw drawer(&drawing, options);
        tree.calculate_xy(options);            // Calculate coords of each point
        drawer.draw_tree(tree, disp_label);
        drawing.autoscale({ 20, 20, 20, 20 }); // 20px margins on all sides
        return drawing;
    }
}