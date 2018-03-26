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
    double x_sep;
    double y_sep;
    int node_size;
    std::string edge_color;
    std::string edge_width;
};

const DrawOpts DEFAULT_DRAWING_OPTIONS = { 10, 10, 3, "black", "1px" };

/** Used for positioning text labels */
enum NodePosition { LEFT, MIDDLE, RIGHT };

/** Abstract base class for trees */
class TreeBase {
public:
    std::string label = "";
    double x = 0;
    double y = 0;
    double displacement = 0;

    virtual TreeBase* left() = 0;
    virtual TreeBase* right() = 0;
    virtual void calculate_xy(const unsigned int, const double, const DrawOpts&) = 0;
    void calculate_displacement();
    static double distance_between(TreeBase* left, TreeBase *right);

private:
    virtual void merge_subtrees(double displacement) = 0;
};

/** A node for a binary tree */
class TreeNode: public TreeBase {
public:
    TreeNode* left() override { return this->left_ptr.get(); }
    TreeNode* right() override { return this->right_ptr.get(); }
    bool is_leaf() { return !this->left() && !this->right(); }

    TreeNode* add_left(const std::string& label = "") {
        this->left_ptr = std::make_shared<TreeNode>(TreeNode());
        this->left_ptr->label = label;
        return this->left();
    };

    TreeNode* add_right(const std::string& label = "") {
        this->right_ptr = std::make_shared<TreeNode>(TreeNode());
        this->right_ptr->label = label;
        return this->right();
    };

    void calculate_xy(const unsigned int depth = 0, const double offset=0,
                      const DrawOpts& options=DEFAULT_DRAWING_OPTIONS) override;

private:
    std::shared_ptr<TreeNode> left_ptr = nullptr;
    std::shared_ptr<TreeNode> right_ptr = nullptr;

    void merge_subtrees(double displacement) override;
};

/** Class for building incomplete binary trees */
class IncompleteBinaryTree {
public:
    IncompleteBinaryTree(): generator(std::random_device()()), distribution(0, 1) {};
    TreeNode make_tree(int depth);
    void make_tree(TreeNode& tree, int depth);

private:
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
};

void left_contour(SVG::SVG& root);
void binary_tree(TreeNode* tree, int depth);
void draw_tree(SVG::SVG& svg, TreeNode& tree, const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);
void label_tree_disp(TreeNode* tree);
SVG::SVG draw_binary_tree(int depth, const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);

#endif //TREE_DRAWING_TREE_H