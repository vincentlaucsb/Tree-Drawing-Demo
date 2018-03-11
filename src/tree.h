//
// Defines a general purpose tree data structure
//

#ifndef TREE_DRAWING_TREE_H
#define TREE_DRAWING_TREE_H

#include "svg/src/svg.hpp"
#include <cassert>
#include <vector>
#include <memory>
#include <map>
#include <math.h>
#include <random>

struct DrawOpts {
    float x_sep;
    float y_sep;
    int node_size;
    std::string edge_color;
    std::string edge_width;
};

const DrawOpts DEFAULT_DRAWING_OPTIONS = { 10, 10, 3, "black", "1px" };

/** Abstract base class for trees */
class TreeBase {
public:
    float x = 0;
    float y = 0;
    float displacement = 0;

    virtual TreeBase* left() = 0;
    virtual TreeBase* right() = 0;
    virtual void calculate_xy(const unsigned int, const float, const DrawOpts&) = 0;
    void calculate_displacement();
    static std::map<int, float> cumulative_displacement(const std::vector<TreeBase*>& node_list);
    static float distance_between(TreeBase* left, TreeBase *right);

    std::vector<TreeBase*> left_contour();
    std::vector<TreeBase*> right_contour();

protected:
    void left_contour(int depth, std::vector<TreeBase*>& node_list);
    void right_contour(int depth, std::vector<TreeBase*>& node_list);

private:
    virtual void merge_subtrees(float displacement) = 0;
};

/** A node for a binary tree */
class TreeNode: public TreeBase {
public:
    inline TreeNode* left() override { return this->left_ptr.get(); }
    inline TreeNode* right() override { return this->right_ptr.get(); }
    inline TreeNode* add_left() {
        this->left_ptr = std::make_shared<TreeNode>(TreeNode());
        return this->left();
    };
    inline TreeNode* add_right() {
        this->right_ptr = std::make_shared<TreeNode>(TreeNode());
        return this->right();
    };

    void calculate_xy(const unsigned int depth = 0, const float offset=0,
                      const DrawOpts& options=DEFAULT_DRAWING_OPTIONS) override;

private:
    std::shared_ptr<TreeNode> left_ptr = nullptr;
    std::shared_ptr<TreeNode> right_ptr = nullptr;

    void merge_subtrees(float displacement) override;
};

/** Class for building incomplete binary trees */
class IncompleteBinaryTree {
public:
    IncompleteBinaryTree(): generator(std::random_device()()), distribution(0, 1) {};
    TreeNode make_tree(int depth);
    void make_tree(TreeNode& tree, int depth);

private:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution;
};

void binary_tree(TreeNode* tree, int depth);
void draw_tree(SVG::Group* edges, SVG::Group* vertices, TreeNode& tree,
               const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);
SVG::SVG draw_binary_tree(int depth, const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);

#endif //TREE_DRAWING_TREE_H