#include "tree.h"
#include <math.h>
#include <iostream>
#define X_SEPARATION 50
#define Y_SEPARATION 50

void TreeNode::calculate_xy(const unsigned int depth, const float offset) {
    /** Calculate the x, y coordinates of each node via a preorder traversal
     *  Also return the width of the final image
     */

    if (depth == 0) this->calculate_displacement();

    this->x = offset + (displacement * X_SEPARATION);
    this->y = depth * Y_SEPARATION;

    if (left) left->calculate_xy(depth + 1, this->x);
    if (right) right->calculate_xy(depth + 1, this->x);
}

void TreeNode::calculate_displacement() {
    /** Calculate the displacement of each node via a postorder traversal */
    this->merge_subtrees(0);
}

float TreeNode::distance_between() {
    /** Return the minimum distance required between two subtrees
     *  such that they have a horizontal distance of two
     */

    // This function should never be called on null nodes
    assert(left != nullptr);
    assert(right != nullptr);

    // First, get cumulative displacement
    // first = depth, second = displacement
    std::map<int, int> left_cd, right_cd;
    int left_max_depth = 0, right_max_depth = 0;

    // Scan left subtree
    auto left_side = this->right_contour();
    if (!left_side.empty()) {
        left_cd[0] = left_side[0]->displacement;
        for (size_t i = 1; i < left_side.size(); i++) {
            left_cd[i] = left_cd[i - 1] + left_side[i]->displacement;
            left_max_depth++;
        }
    }

    // Scan right subtree
    auto right_side = this->left_contour();
    if (!right_side.empty()) {
        right_cd[0] = right_side[0]->displacement;
        for (size_t i = 1; i < right_side.size(); i++) {
            right_cd[i] = right_cd[i - 1] + right_side[i]->displacement;
            right_max_depth++;
        }
    }

    int max_depth = std::min(left_max_depth, right_max_depth);

    // Right cumulative displacement should be negative, because we're scanning the left contour
    // Left cumulative displacement should be positive, because we're scanning the right contour
    assert(left_cd[max_depth] >= 0);
    assert(right_cd[max_depth] <= 0);

    auto ret_val = left_cd[max_depth] + abs(right_cd[max_depth]);
    return (ret_val);
}

void TreeNode::merge_subtrees(float displacement) {
    /** "Merge" the subtrees of this node such that they have a horizontal separation of 2
     *  by setting an appropriate displacement for this node
     *
     *  displacement: Default displacement value if this is a leaf node
     *  - Should be -1 for left nodes and 1 for right nodes
     */

    // Set displacements for edge cases
    if (!this->left && !this->right) { // Base Case: This is a leaf node (i.e. no children)
        this->displacement = displacement;
        return;
    }
    else if (!this->left || !this->right) { // Edge Case: One child NULL
        this->displacement = 0.0;
    }

    // Postorder traversal
    if (this->left) this->left->merge_subtrees(-1.0);
    if (this->right) this->right->merge_subtrees(1.0);

    // Merge subtrees (if they exist)
    if (this->left && this->right) {
        // Temporary values
        this->left->displacement = -1;
        this->right->displacement = 1;

        // Move subtrees
        int subtree_separation = this->distance_between()/2;
        this->left->displacement = -subtree_separation;
        this->right->displacement = subtree_separation;

        // Place this node halfway between its children x-coordinate wise
        this->displacement = 0;
    }
}

std::vector<TreeNode*> TreeNode::left_contour() {
    /** Return a list with the left contour of a vertex */
    std::vector<TreeNode*> ret;
    for (TreeNode* node = this->left; node != nullptr; node = node->left) {
        ret.push_back(node);
    }
    return ret;
}

std::vector<TreeNode*> TreeNode::right_contour() {
    /** Return a list with the right contour of a vertex */
    std::vector<TreeNode*> ret;
    for (TreeNode* node = this->right; node != nullptr; node = node->right) {
        ret.push_back(node);
    }
    return ret;
}