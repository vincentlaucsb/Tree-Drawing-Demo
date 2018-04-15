// This file contains only the tree drawing algorithm itself

#include "tree.h"
#define MINIMUM_SEPARATION 1

namespace tree {
    void TreeBase::calculate_xy(const DrawOpts& options, const unsigned int depth, const double offset) {
        /** Calculate the x, y coordinates of each node via a preorder traversal */
        if (depth == 0) this->calculate_displacement();

        this->x = offset + (displacement * options.x_sep);
        this->y = depth * options.y_sep;

        for (auto& child : this->get_children())
            if (child != this->thread) child->calculate_xy(options, depth + 1, this->x);
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
        double left_dist = abs(left->displacement), right_dist = abs(right->displacement),
            cursep = left_dist + right_dist;
        double temp_ldist, temp_rdist;

        // Accumulate displacements and terminate when either subtree runs out of height
        while (left && right) {
            // Add extra distance when subtrees get too close
            if (cursep < MINIMUM_SEPARATION) {
                left_dist += (MINIMUM_SEPARATION - cursep) / 2;
                right_dist += (MINIMUM_SEPARATION - cursep) / 2;
                cursep = MINIMUM_SEPARATION;
            }

            if (left->right()) {
                left = left->right();
                temp_ldist = left->displacement;
                if (left->thread_roffset != 0) {
                    std::cout << "Following thread" << std::endl;
                    temp_ldist = left->thread_roffset;
                }
                cursep -= temp_ldist;
            }
            else if (left->left()) {
                left = left->left();
                temp_ldist = left->displacement;
                if (left->thread_loffset != 0) {
                    std::cout << "Following thread" << std::endl;
                    temp_ldist = left->thread_loffset;
                }
                cursep -= temp_ldist;
            }
            else {
                left = nullptr;
            }

            if (right->left()) {
                right = right->left();
                temp_rdist = right->displacement;
                if (right->thread_loffset != 0) {
                    std::cout << "Following thread" << std::endl;
                    temp_rdist = right->thread_loffset;
                }
                cursep += temp_rdist;
            }
            else if (right->right()) {
                right = right->right();
                temp_rdist = right->displacement;
                if (right->thread_roffset != 0) {
                    std::cout << "Following thread" << std::endl;
                    temp_rdist = right->thread_roffset;
                }
                cursep += temp_rdist;
            }
            else { right = nullptr;  }
        }

        // Perform threading if necessary
        if (left) {
            auto rmost = rroot->right_most();
            if (left->thread != rmost.addr) {
                rmost.addr->thread = left;
                rmost.addr->thread->thread_loffset = -abs((rmost.addr->displacement + lroot->displacement) - left_dist);
            }
        }
        if (right) {
            auto lmost = lroot->left_most();
            if (right->thread != lmost.addr) {
                lmost.addr->thread = right;
                lmost.addr->thread->thread_roffset = abs((lmost.addr->displacement + rroot->displacement) - right_dist);
            }
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
        if (this->left()) this->left()->merge_subtrees(-MINIMUM_SEPARATION/2);
        if (this->right()) this->right()->merge_subtrees(MINIMUM_SEPARATION/2);

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
        if (this->left() && !this->thread) this->left()->left_most(current, lmost);
        if (this->right() && !this->thread) this->right()->left_most(current, lmost);
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
        if (this->right() && !this->thread) this->right()->right_most(current, rmost);
        if (this->left() && !this->thread) this->left()->right_most(current, rmost);
    }
}