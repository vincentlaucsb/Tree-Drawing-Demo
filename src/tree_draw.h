#pragma once
#include "tree.h"
#include "nary_tree.h"

namespace tree {
    namespace helpers {
        void binary_tree(TreeNode* tree, int depth);
        void nary_tree(NaryTreeNode* tree, int n, int height);

        class TreeDraw {
            /** A class used for drawing trees */
        public:
            TreeDraw(SVG::SVG* _tree_svg, const DrawOpts& _options);
            void draw_tree(TreeBase& tree, bool disp_label = false);
        protected:
            SVG::SVG* tree_svg;
            DrawOpts options;
            SVG::Group* edges = nullptr;
            SVG::Group* vertices = nullptr;
        };
    }

    class IncompleteBinaryTree {
        /** Class for building incomplete binary trees */
    public:
        IncompleteBinaryTree() : generator(std::random_device()()), distribution(0, 1) {};
        TreeNode make_tree(int depth);
        void make_tree(TreeNode& tree, int depth);

    private:
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution;
    };
    
    class IncompleteNaryTree {
        /** Class for building complete nary trees */
    public:
        IncompleteNaryTree(double mean, double var) :
            generator(std::random_device()()),
            distribution(mean, var) {};

        NaryTreeNode make_tree(int depth) {
            /** Generate an incomplete binary tree recursively */
            NaryTreeNode tree;
            make_tree_helper(tree, depth);
            return tree;
        }

        void make_tree_helper(NaryTreeNode& tree, int depth) {
            if (depth) {
                int nodes = distribution(generator);
                for (int i = 0; i < (int)nodes; i++) {
                    auto child = tree.add_child();
                    this->make_tree_helper(*child, depth - 1);
                }
            }
        }

    private:
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
    };

    void left_contour(SVG::SVG& root);
    void right_contour(SVG::SVG& root);

    TreeNode binary_tree(int height);
    NaryTreeNode nary_tree(int n, int height);
    SVG::SVG draw_tree(TreeBase& tree, const DrawOpts& options, bool disp_label = false);
}