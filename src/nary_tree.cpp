//
// Contains code for drawing trees with arbitrarily many nodes
//

#include "nary_tree.h"

void NaryTreeNode::calculate_xy(const unsigned int depth, const double offset, const DrawOpts& options) {
    /** Calculate the x, y coordinates of each node via a preorder traversal */
    if (depth == 0) this->calculate_displacement();

    this->x = offset + (displacement * options.x_sep);
    this->y = depth * options.y_sep;

    for (auto& child: this->children)
        child->calculate_xy(depth + 1, this->x, options);
}

void NaryTreeNode::merge_subtrees(double displacement) {
    /** "Merge" the subtrees of this node such that they have a horizontal separation of 2
    *  by setting an appropriate displacement for this node
    *
    *  displacement: Default displacement value if this is a leaf node
    */

    // Default displacement
    this->displacement = displacement;

    // Postorder traversal
    double width = (double)this->children.size() - 1;
    for (size_t i = 0; i < this->children.size(); i++)
        this->children[i]->merge_subtrees(-width / 2 + i);

    // Merge subtrees (if they exist) by moving from left to right, merging two
    // adjacent subtrees at a time
    if (!this->children.empty()) {
        // Because by default, this node has displacement zero,
        // it will be centered over its children
        double total_width = 0;
        std::vector<double> subtree_separation;
        for (size_t i = 0; (i + 1) < this->children.size(); i++) {
            double separation = this->distance_between(this->children[i].get(),
                this->children[i + 1].get());
            total_width += separation;
            subtree_separation.push_back(separation);
        }

        double current_position = -total_width / 2;
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->displacement = current_position;
            if (i + 1 < this->children.size()) current_position += subtree_separation[i];
        }
    }
}