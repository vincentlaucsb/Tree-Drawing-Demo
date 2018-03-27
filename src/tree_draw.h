#pragma once
#include "tree.h"
#include "nary_tree.h"

class TreeDraw {
    /** A class used for drawing trees */
public:
    TreeDraw(SVG::SVG* _tree_svg, const DrawOpts& _options);
    void draw_tree(TreeNode& tree, bool disp_label = false);
    void draw_tree(NaryTreeNode& tree, bool disp_label = false);
    static void label_disp(TreeNode& tree);
    static void label_disp(NaryTreeNode& tree);
protected:
    SVG::SVG* tree_svg;
    DrawOpts options;
    SVG::Group* edges = nullptr;
    SVG::Group* vertices = nullptr;
};

void left_contour(SVG::SVG& root);
void right_contour(SVG::SVG& root);
void binary_tree(TreeNode* tree, int depth);

NaryTreeNode nary_tree(int n, int height);
void nary_tree(NaryTreeNode* tree, int n, int height);

template<typename T>
inline SVG::SVG draw_tree(T tree, const DrawOpts& options, bool disp_label=false) {
    /** Create an SVG drawing of the given binary or nary tree */
    SVG::SVG drawing;
    TreeDraw drawer(&drawing, options);
    drawer.draw_tree(tree, disp_label);
    drawing.autoscale({ 20, 20, 20, 20 }); // 20px margins on all sides
    return drawing;
}

SVG::SVG draw_binary_tree(const int height, const DrawOpts& options, bool disp_label=false);
SVG::SVG draw_nary_tree(const int n, const int height, const DrawOpts& options, bool disp_label=false);