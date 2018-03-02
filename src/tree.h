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
    int x_sep;
    int y_sep;
    int node_size;
    std::string edge_color;
    std::string edge_width;
};

const DrawOpts DEFAULT_DRAWING_OPTIONS = { 10, 10, 3, "black", "1px" };

class TreeNode {
public:
    float x;
    float y;
    float displacement = 0;

    TreeNode* left = nullptr;
    TreeNode* right = nullptr;        

    static float distance_between(TreeNode* left, TreeNode *right);
    static std::map<int, float> cumulative_displacement(const std::vector<TreeNode*>& node_list);
    void calculate_xy(const unsigned int depth = 0, const float offset=0,
                      const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);
    void calculate_displacement();
    std::vector<TreeNode*> left_contour();
    std::vector<TreeNode*> right_contour();

private:
    void merge_subtrees(float displacement);
    void left_contour(int depth, std::vector<TreeNode*>& node_list);
    void right_contour(int depth, std::vector<TreeNode*>& node_list);
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
void draw_tree(SVG::Group& edges, SVG::Group& vertices, TreeNode& tree);
SVG::SVG draw_binary_tree(int depth, const DrawOpts& options=DEFAULT_DRAWING_OPTIONS);

#endif //TREE_DRAWING_TREE_H