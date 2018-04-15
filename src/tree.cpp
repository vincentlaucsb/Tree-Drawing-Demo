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

    double TreeBase::distance_between(TreeBase* left, TreeBase* right) {
        /* Return the minimum horizontal distance needed between two subtrees
         * such that they can be placed 2 units apart horizontally
         *
         * Precondition: All nodes except the top nodes have correct displacements set
         */

        TreeBase *lroot = left, *rroot = right;
        double left_dist = 0, right_dist = 0;
        int height = 0;

        // Accumulate displacements and terminate when either subtree runs out of height
        while (left && right) {
            double temp_ldist = left->displacement, temp_rdist = right->displacement;
            if (left->thread_loffset) temp_ldist = left->thread_loffset;
            if (right->thread_roffset) temp_rdist = right->thread_roffset;

            if ((height > 0) || ((temp_ldist <= 0) && temp_rdist >= 0)) {
                if (left->displacement < 0) left_dist += abs(temp_ldist);
                else right_dist += temp_ldist;

                if (right->displacement < 0) left_dist += abs(temp_rdist);
                else right_dist += temp_rdist;
            }
            else {
                // Fixes an edge case when laying leaf nodes of odd n-ary trees
                // Choice of adding to left_dist (instead of right_dist) was arbitrary
                left_dist += abs(temp_ldist - temp_rdist);
            }

            if (left->right()) left = left->right();
            else left = left->left();

            if (right->left()) right = right->left();
            else right = right->right();

            height++;
        }

        // Perform threading if necessary
        if (left) {
            auto rmost = rroot->right_most();
            rmost.addr->thread = left;
            rmost.addr->thread_loffset = left_dist + rmost.displacement;
        }
        if (right) {
            auto lmost = lroot->left_most();
            lmost.addr->thread = right;
            lmost.addr->thread_roffset = right_dist + lmost.displacement;
        }

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

    TreeBase::Extreme TreeBase::left_most() {
        /** Return the left most node in a subtree */
        std::vector<Extreme> lmost;
        this->left_most(Extreme({ this, 0, 0 }), lmost);
        return lmost.back();
    }

    void TreeBase::left_most(Extreme& current, std::vector<Extreme>& lmost) {
        current.addr = this;
        current.displacement += this->displacement;
        current.level++;
        if (lmost.size() <= current.level) lmost.push_back(current);
        if (this->left()) this->left()->left_most(current, lmost);
        if (this->right()) this->right()->left_most(current, lmost);
    }

    TreeBase::Extreme TreeBase::right_most() {
        /** Return a list with the right contour of a vertex */
        std::vector<Extreme> rmost;
        this->right_most(Extreme({ this, 0, 0 }), rmost);
        return rmost.back();
    }

    void TreeBase::right_most(Extreme& current, std::vector<Extreme>& rmost) {
        current.addr = this;
        current.displacement += this->displacement;
        current.level++;
        if (rmost.size() <= current.level) rmost.push_back(current);
        if (this->right()) this->right()->right_most(current, rmost);
        if (this->left()) this->left()->right_most(current, rmost);
    }
}