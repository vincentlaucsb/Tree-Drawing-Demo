//
// Defines a general purpose tree data structure
//

#ifndef TREE_DRAWING_TREE_H
#define TREE_DRAWING_TREE_H

#include "svg/src/svg.hpp"
#include <cassert>
#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <math.h>
#include <random>

namespace tree {
    struct DrawOpts {
        double x_sep;
        double y_sep;
        int node_size;
        std::string edge_color;
        std::string edge_width;
    };

    const DrawOpts DEFAULT_DRAWING_OPTIONS = { 10, 10, 3, "black", "1px" };

    class TreeBase;
    using NodeList = std::vector<TreeBase*>;

    /** Used for positioning text labels */
    enum NodePosition { LEFT, MIDDLE, RIGHT };

    /** Abstract base class for trees */
    class TreeBase {
    public:
        struct Extreme {
            TreeBase* addr;
            double displacement = 0;
            double level;
        };

        std::string label = "";
        double x = 0;
        double y = 0;
        double displacement = 0;
        TreeBase * thread_l = nullptr;
        TreeBase * thread_r = nullptr;
        double thread_loffset = 0;
        double thread_roffset = 0;

        virtual TreeBase* left() = 0;
        virtual TreeBase* right() = 0;
        virtual NodeList get_children() = 0;
        virtual bool is_leaf() = 0;

        void calculate_xy(const DrawOpts&, const unsigned int = 0, const double = 0);
        void calculate_displacement();
        static double distance_between(TreeBase*, TreeBase*);
        Extreme left_most();
        Extreme right_most();

    protected:
        virtual void merge_subtrees(double displacement) = 0;

    private:
        void left_most(Extreme&, std::vector<Extreme>&);
        void right_most(Extreme&, std::vector<Extreme>&);
    };

    /** A node for a binary tree */
    class TreeNode : public TreeBase {
    public:
        TreeNode* left() override { return this->left_ptr ? this->left_ptr.get() : (TreeNode*)this->thread_l; }
        TreeNode* right() override { return this->right_ptr ? this->right_ptr.get() : (TreeNode*)this->thread_r; }
        bool is_leaf() override { return !this->left() && !this->right(); }

        TreeNode* add_left(TreeNode node = TreeNode(), const std::string& label = "") {
            this->left_ptr = std::make_shared<TreeNode>(node);
            this->left_ptr->label = label;
            return this->left();
        };

        TreeNode* add_right(TreeNode node = TreeNode(), const std::string& label = "") {
            this->right_ptr = std::make_shared<TreeNode>(node);
            this->right_ptr->label = label;
            return this->right();
        };

        NodeList get_children() override {
            NodeList ret = {};
            if (left()) { ret.push_back(left()); }
            if (right()) { ret.push_back(right()); }
            return ret;
        }

    private:
        std::shared_ptr<TreeNode> left_ptr = nullptr;
        std::shared_ptr<TreeNode> right_ptr = nullptr;
        void merge_subtrees(double displacement) override;
    };
}

#endif //TREE_DRAWING_TREE_H