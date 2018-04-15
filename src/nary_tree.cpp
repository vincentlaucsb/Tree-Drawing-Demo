//
// Contains code for drawing trees with arbitrarily many nodes
//

#include "nary_tree.h"

namespace tree {
    void NaryTreeNode::merge_subtrees(double displacement) {
        /** "Merge" the subtrees of this node such that they have a horizontal separation of 2
        *  by setting an appropriate displacement for this node
        *
        *  displacement: Default displacement value if this is a leaf node
        */

        // Default displacement
        //this->displacement = displacement;

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
}