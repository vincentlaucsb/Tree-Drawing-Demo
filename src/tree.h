//
// Defines a general purpose tree data structure
//

#ifndef TREE_DRAWING_TREE_H
#define TREE_DRAWING_TREE_H

#include <cassert>
#include <vector>
#include <memory>
#include <map>
#include <math.h>

class TreeNode {
public:
    int x;
    int y;

    float displacement = NAN;

    TreeNode* left = nullptr;
    TreeNode* right = nullptr;        

    static float distance_between(TreeNode* left, TreeNode *right);
    static std::map<int, float> cumulative_displacement(const std::vector<TreeNode*>& node_list);
    void calculate_xy(const unsigned int depth = 0, const float offset=0);
    void calculate_displacement();
    std::vector<TreeNode*> left_contour();
    std::vector<TreeNode*> right_contour();

private:
    void merge_subtrees(float displacement);
    float distance_between();

    void left_contour(int depth, std::vector<TreeNode*>& node_list);
    void right_contour(int depth, std::vector<TreeNode*>& node_list);
};

void binary_tree(TreeNode* tree, int depth);

#endif //TREE_DRAWING_TREE_H