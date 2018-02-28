// Tree drawing example of a binary tree with uniform width and height

#include "svg/src/svg.hpp"
#include "tree.h"
#include <fstream>

void draw_tree(SVG::Group& edges, SVG::Group& vertices, TreeNode& tree) {
    // Draw an SVG tree recursively
    vertices.add_child(SVG::Circle(tree.x, tree.y, 3 ));
    if (tree.left) {
        draw_tree(edges, vertices, *(tree.left));
        edges.add_child(SVG::Line(tree.x, tree.left->x, tree.y, tree.left->y));
    }

    if (tree.right) {
        draw_tree(edges, vertices, *(tree.right));
        edges.add_child(SVG::Line(tree.x, tree.right->x, tree.y, tree.right->y));
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [output file] [depth]" << std::endl
            << "Produces a binary tree of specified depth" << std::endl;
        return 1;
    }

    //IncompleteBinaryTree tree_maker;
    //TreeNode tree = tree_maker.make_tree(std::stoi(argv[2]));
    TreeNode tree;
    binary_tree(&tree, std::stoi(argv[2]));
    tree.calculate_xy();
    
    SVG::SVG root;
    SVG::Group edges, nodes;
    edges.set_attr("stroke", "black")
         .set_attr("stroke-width", "1px");

    std::ofstream outfile(argv[1]);
    draw_tree(edges, nodes, tree);
    root.add_child(edges, nodes);
    root.set_bbox();
    outfile << root.to_string();
    
    return 0;
}