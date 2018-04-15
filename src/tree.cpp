// This file contains only the tree drawing algorithm itself

#include "tree.h"
#define MINIMUM_SEPARATION 2
#define SEP_DEBUG std::cout << "Following thread: cursep " << cursep

namespace tree {
    void TreeBase::calculate_xy(const DrawOpts& options, const unsigned int depth, const double offset) {
        /** Calculate the x, y coordinates of each node via a preorder traversal */
        if (depth == 0) this->calculate_displacement();

        this->x = offset + (displacement * options.x_sep);
        this->y = depth * options.y_sep;

        for (auto& child : this->get_children())
            if (child != this->thread_l) child->calculate_xy(options, depth + 1, this->x);
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
                if (left->thread_r) {
                    SEP_DEBUG << left->thread_roffset << std::endl;
                    temp_ldist = left->thread_roffset;
                }
                else {
                    temp_ldist = left->right()->displacement;
                }
                
                left = left->right();
                cursep -= temp_ldist;
            }
            else if (left->left()) {
                if (left->thread_l) {
                    SEP_DEBUG << left->thread_loffset << std::endl;
                    temp_ldist = left->thread_loffset;
                }
                else {
                    temp_ldist = left->left()->displacement;
                }
                left = left->left();
                cursep -= temp_ldist;
            }
            else {
                left = nullptr;
            }

            if (right->left()) {
                if (right->thread_l) {
                    SEP_DEBUG << right->thread_loffset << std::endl;
                    temp_rdist = right->thread_loffset;
                }
                else {
                    temp_rdist = right->left()->displacement;
                }
                right = right->left();
                cursep += temp_rdist;
            }
            else if (right->right()) {
                if (right->thread_r) {
                    SEP_DEBUG << right->thread_roffset << std::endl;
                    temp_rdist = right->thread_roffset;
                }
                else {
                    temp_rdist = right->right()->displacement;
                }
                right = right->right();
                cursep += temp_rdist;
            }
            else { right = nullptr;  }
        }

        // Perform threading if necessary
        if (left) {
            auto rmost = rroot->right_most();
            if (left->thread_l != rmost.addr) {
                rmost.addr->thread_l = left;
                rmost.addr->thread_loffset = -abs((rmost.addr->displacement + lroot->displacement) - left_dist);
            }
        }
        if (right) {
            auto lmost = lroot->left_most();
            if (right->thread_r != lmost.addr) {
                lmost.addr->thread_r = right;
                lmost.addr->thread_roffset = abs((lmost.addr->displacement + rroot->displacement) - right_dist);
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