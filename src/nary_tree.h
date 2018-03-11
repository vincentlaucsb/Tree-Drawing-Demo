//
// Created by vince on 3/3/2018.
//

#ifndef TREE_DRAWING_NARY_TREE_H
#define TREE_DRAWING_NARY_TREE_H
#include "tree.h"

class NaryTreeNode: public TreeBase {
public:
    inline NaryTreeNode* left() override {
        if (this->children.empty()) return nullptr;
        return this->children[0].get();
    }

    inline NaryTreeNode* right() override {
        if (this->children.empty()) return nullptr;
        return this->children[this->children.size() - 1].get();
    }

    inline NaryTreeNode* add_child() {
        this->children.push_back(std::make_shared<NaryTreeNode>(NaryTreeNode()));
        return this->children[this->children.size() - 1].get();
    };

    void calculate_xy(const unsigned int depth = 0, const float offset=0,
                      const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);
    std::vector<std::shared_ptr<NaryTreeNode>> children;

private:
    void merge_subtrees(float displacement) override;
};

NaryTreeNode nary_tree(int n, int height);
void nary_tree(NaryTreeNode* tree, int n, int height);

void draw_tree(SVG::Group* edges, SVG::Group* vertices, NaryTreeNode& tree);
SVG::SVG draw_nary_tree(const int, const int, const DrawOpts&);

#endif //TREE_DRAWING_NARY_TREE_H