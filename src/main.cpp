// Tree drawing example of a binary tree with uniform width and height

#include "tree.h"
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [output file] [depth]" << std::endl
            << "Produces a binary tree of specified depth" << std::endl;
        return 1;
    }

    int depth = std::stoi(argv[2]);

    SVG::SVG tree_drawing = draw_binary_tree(depth);
    std::ofstream outfile(argv[1]);
    outfile << tree_drawing.to_string();
    
    return 0;
}