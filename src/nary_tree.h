//
// Created by vince on 3/3/2018.
//

#ifndef TREE_DRAWING_NARY_TREE_H
#define TREE_DRAWING_NARY_TREE_H
#include "tree.h"

namespace tree {
    class NaryTreeNode : public TreeBase {
    public:
        inline NaryTreeNode* left() override {
            if (this->children.empty()) return (NaryTreeNode*)this->thread_l;
            return this->children[0].get();
        }

        inline NaryTreeNode* right() override {
            if (this->children.empty()) return (NaryTreeNode*)this->thread_r;
            return this->children[this->children.size() - 1].get();
        }

        inline NaryTreeNode* add_child() {
            this->children.push_back(std::make_shared<NaryTreeNode>(NaryTreeNode()));
            return this->children[this->children.size() - 1].get();
        };

        bool is_leaf() override { return this->children.empty(); }

        NodeList get_children() override {
            NodeList ret;
            for (auto& child : children) ret.push_back(child.get());
            if (ret.empty()) {
                if (this->thread_l) ret.push_back(thread_l);
                if (this->thread_r) ret.push_back(thread_r);
            }
            return ret;
        }

        std::vector<std::shared_ptr<NaryTreeNode>> children;

    private:
        void merge_subtrees(double displacement) override;
    };
}

#endif //TREE_DRAWING_NARY_TREE_H