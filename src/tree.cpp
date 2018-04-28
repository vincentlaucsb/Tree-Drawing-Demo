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

    size_t TreeBase::height() {
        /** Calculate the height of this tree */
        size_t height = 1;
        std::deque<TreeBase*> children;
        for (auto& child : this->get_children()) {
            height = std::max(height, child->height() + 1);
        }

        return height;
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

        /*
         * left_dist, right_dist: Used for calculating total width of left and right subtrees respectively
         * left_sum, right_sum: Current accumulated offsets; used for setting thread offsets
         * cursep: The current separation between subtrees; used as a trigger to add extra distance
         */
        double left_dist = abs(left->displacement), right_dist = abs(right->displacement),
            left_sum = 0, right_sum = 0,
            cursep = left_dist + right_dist;

        // Accumulate displacements and terminate when either subtree runs out of height
        while (left && right) {
            double temp_ldist = 0, temp_rdist = 0;

            // Add extra distance when subtrees get too close
            if (cursep < MINIMUM_SEPARATION) {
                left_dist += (MINIMUM_SEPARATION - cursep) / 2;
                right_dist += (MINIMUM_SEPARATION - cursep) / 2;
                cursep = MINIMUM_SEPARATION;
            }

            if (left->right()) {
                temp_ldist = left->right_offset();
                cursep -= temp_ldist;
                left = left->right();
            }
            else if (left->left()) {
                temp_ldist = left->left_offset();
                cursep += temp_ldist;
                left = left->left();
            }
            else {
                left = nullptr;
            }

            if (right->left()) {
                temp_rdist = right->left_offset();
                cursep += right->left_offset();
                right = right->left();
            }
            else if (right->right()) {
                temp_rdist = right->right_offset();
                cursep += right->right_offset();
                right = right->right();
            }
            else { right = nullptr;  }

            left_sum += temp_ldist;
            right_sum += temp_rdist;
        }

        left_sum -= left_dist;
        right_sum += right_dist;

        // Perform threading if necessary
        if (left) {
            auto rmost = rroot->right_most(Extreme({ this, this->right_offset(), 0 }));
            if (left->thread_l != rmost.addr) {
                rmost.addr->thread_l = left;
                if (rmost.displacement < left_sum) { // Item on left subtree actually further right than rmost
                    rmost.addr->thread_loffset = abs(rmost.displacement - left_sum);
                }
                else {
                    rmost.addr->thread_loffset = -abs(left_sum - rmost.displacement);
                }
                
            }
        }
        if (right) {
            auto lmost = lroot->left_most(Extreme({ this, this->left_offset(), 0 }));
            if (right->thread_r != lmost.addr) {
                lmost.addr->thread_r = right;
                if (lmost.displacement > right_sum) { // Item on right subtree actually further left than lmost
                    lmost.addr->thread_roffset = -abs(lmost.displacement - right_sum);
                }
                else {
                    lmost.addr->thread_roffset = abs(right_sum - lmost.displacement);
                }
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

    TreeBase::Extreme TreeBase::left_most(Extreme& current) {
        /** Return the left most node in a subtree */
        std::vector<Extreme> lmost;
        this->left_most(current, lmost);
        return lmost.back();
    }

    void TreeBase::left_most(Extreme& current, std::vector<Extreme>& lmost) {
        current.addr = this;
        current.level++;
        if (lmost.size() <= current.level) lmost.push_back(current);
        if (this->left()) {
            current.displacement += this->left_offset();
            this->left()->left_most(current, lmost);
        }
        if (this->right()) {
            current.displacement += this->right_offset();
            this->right()->left_most(current, lmost);
        }
    }

    TreeBase::Extreme TreeBase::right_most(Extreme& current) {
        /** Return a list with the right contour of a vertex */
        std::vector<Extreme> rmost;
        this->right_most(current, rmost);
        return rmost.back();
    }

    void TreeBase::right_most(Extreme& current, std::vector<Extreme>& rmost) {
        current.addr = this;
        current.level++;
        if (rmost.size() <= current.level) rmost.push_back(current);
        if (this->right()) {
            current.displacement += this->right_offset();
            this->right()->right_most(current, rmost);
        }
        if (this->left()) {
            current.displacement += this->left_offset();
            this->left()->right_most(current, rmost);
        }
    }
}