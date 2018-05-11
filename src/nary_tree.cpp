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
        double width = ((double)this->children.size() - 1) * MINIMUM_SEPARATION,
            current_width = -width / 2;
        for (size_t i = 0; i < this->children.size(); i++) {
            this->children[i]->merge_subtrees(current_width);
            current_width += width / (double)(this->children.size() - 1);
        }

        // Merge subtrees (if they exist) by moving from left to right, merging two
        // adjacent subtrees at a time
        if (!this->children.empty()) {
            // Because by default, this node has displacement zero,
            // it will be centered over its children
            for (size_t i = 0; (i + 1) < this->children.size(); i++) {
                if (this->label == "root") std::cout << "Merging trees" << std::endl;
                auto dist = this->distance_between(this->children[i].get(), this->children[i + 1].get());
                this->children[i + 1]->displacement = i * dist.first;

                // Thread if necessary
                if (dist.second.left) this->thread_left(dist.second);
                if (dist.second.right) this->thread_right(dist.second);
            }
        }
    }
}