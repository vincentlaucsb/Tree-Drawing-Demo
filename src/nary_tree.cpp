//
// Contains code for drawing trees with arbitrarily many nodes
//

#include "nary_tree.h"

void NaryTreeNode::calculate_xy(const unsigned int depth, const float offset, const DrawOpts& options) {
    /** Calculate the x, y coordinates of each node via a preorder traversal */
    if (depth == 0) this->calculate_displacement();

    this->x = offset + (displacement * options.x_sep);
    this->y = depth * options.y_sep;

    for (auto& child: this->children)
        child->calculate_xy(depth + 1, this->x, options);
}

void NaryTreeNode::merge_subtrees(float displacement) {
    /** "Merge" the subtrees of this node such that they have a horizontal separation of 2
     *  by setting an appropriate displacement for this node
     *
     *  displacement: Default displacement value if this is a leaf node
     *  - Should be -1 for left nodes and 1 for right nodes
     */

    // Set displacements for edge cases
    if (this->children.empty()) {
        // Base Case: This is a leaf node (i.e. no children)
        this->displacement = displacement;
        return;
    }

    // Postorder traversal
    auto width = (float)this->children.size();
    for (size_t i = 0 ; i < this->children.size(); i++)
        this->children[i]->merge_subtrees(-(width/2) + (i*width));

    // Merge subtrees (if they exist) by moving from left to right, merging two
    // adjacent subtrees at a time
    if (!this->children.empty()) {
        // Because by default, this node has displacement zero,
        // it will be centered over its children
        vector<float> subtree_separation;
        float separation = 0;
        for (size_t i = 0; (i + 1) < this->children.size(); i++) {
            separation = this->distance_between(this->children[i].get(),
                                                this->children[i + 1].get());
            if (i > 0) separation += subtree_separation[i - 1];

            subtree_separation.push_back(separation);
        }

        float& total_width = subtree_separation[subtree_separation.size() - 1];
        for (size_t i = 0; i < this->children.size(); i++) {
            if (i == 0) this->children[i]->displacement = -(total_width)/2;
            else this->children[i]->displacement = -(total_width / 2) + subtree_separation[i - 1];
        }
    }
}