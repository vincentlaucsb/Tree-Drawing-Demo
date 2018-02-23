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

  void calculate_xy(const unsigned int depth = 0, const float offset=0);
  void calculate_displacement();

private:

  void merge_subtrees(float displacement);
  float distance_between();

  std::vector<TreeNode*> left_contour();
  std::vector<TreeNode*> right_contour();
};

#endif //TREE_DRAWING_TREE_H