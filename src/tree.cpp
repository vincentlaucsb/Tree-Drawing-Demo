// This file contains only the tree drawing algorithm itself

#include "tree.h"

namespace tree {
    void TreeBase::calculate_xy(const DrawOpts& options, const unsigned int depth, const double offset) {
        /** Calculate the x, y coordinates of each node via a preorder traversal */
        if (depth == 0) this->calculate_displacement();

        this->x = offset + (displacement * options.x_sep);
        this->y = depth * options.y_sep;

        for (auto& child : this->get_children())
            child->calculate_xy(options, depth + 1, this->x);
    }

    void TreeBase::calculate_displacement() {
        /** Calculate the displacement of each node via a postorder traversal */
        this->merge_subtrees(0);
    }

    double TreeBase::distance_between(TreeBase* left_root, TreeBase* right_root) {
        /* Return the minimum horizontal distance needed between two subtrees
         * such that they can be placed 2 units apart horizontally
         *
         * Precondition: All nodes except the top nodes have correct displacements set
         */

        double left_dist = 0, right_dist = 0;
        int height = 0;
        auto left_subtree = left_root->right_contour(),
            right_subtree = right_root->left_contour();

        // Accumulate displacements and terminate when either subtree runs out of height
        auto left = left_subtree.begin(), right = right_subtree.begin();
        while (left != left_subtree.end() && right != right_subtree.end()) {
            // Fixes an edge case when laying leaf nodes of odd n-ary trees
            if ((height > 0) || (((*left)->displacement <= 0) && (*right)->displacement >= 0)) {
                if ((*left)->displacement < 0) left_dist += abs((*left)->displacement);
                else right_dist += (*left)->displacement;

                if ((*right)->displacement < 0) left_dist += abs((*right)->displacement);
                else right_dist += (*right)->displacement;
            }
            else {
                // Choice of adding to left_dist (instead of right_dist) was arbitrary
                left_dist += abs((*left)->displacement - (*right)->displacement);
            }

            left++; right++; height++;
        }

        // Perform threading if necessary
        if (left_subtree.size() < right_subtree.size())
            left_subtree.back()->thread = *right;
        if (right_subtree.size() < left_subtree.size())
            right_subtree.back()->thread = *left;

        return left_dist + right_dist;
    }

    void TreeNode::merge_subtrees(double displacement) {
        /* "Merge" the subtrees of this node such that they have a horizontal separation of 2
         *  by setting an appropriate displacement for this node
         *
         *  displacement: Default displacement value if this is a leaf node
         *  - Should be -1 for left nodes and 1 for right nodes
         */

         // Default displacement
        this->displacement = displacement;

        // Postorder traversal
        if (this->left()) this->left()->merge_subtrees(-1);
        if (this->right()) this->right()->merge_subtrees(1);

        // Merge subtrees (if they exist)
        if (this->left() && this->right()) {
            // Because by default, this node has displacement zero,
            // it will be centered over its children
            double subtree_separation = (this->distance_between(this->left(), this->right())) / 2;
            this->left()->displacement = -subtree_separation;
            this->right()->displacement = subtree_separation;
        }
    }

    NodeList TreeBase::left_contour() {
        /** Return a list with the left contour of a vertex */
        NodeList node_list;
        this->left_contour(0, node_list);
        if (node_list.back()->thread) node_list.push_back(node_list.back()->thread);
        return node_list;
    }

    void TreeBase::left_contour(int depth, NodeList& node_list) {
        if (node_list.size() <= depth) node_list.push_back(this);
        if (this->left()) this->left()->left_contour(depth + 1, node_list);
        if (this->right()) this->right()->left_contour(depth + 1, node_list);
    }

    NodeList TreeBase::right_contour() {
        /** Return a list with the right contour of a vertex */
        NodeList node_list;
        this->right_contour(0, node_list);
        if (node_list.back()->thread) node_list.push_back(node_list.back()->thread);
        return node_list;
    }

    void TreeBase::right_contour(int depth, NodeList& node_list) {
        if (node_list.size() <= depth) node_list.push_back(this);
        if (this->right()) this->right()->right_contour(depth + 1, node_list);
        if (this->left()) this->left()->right_contour(depth + 1, node_list);
    }
}