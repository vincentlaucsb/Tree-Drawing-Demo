// Tree drawing example of a binary tree with uniform width and height

#include "cxxopts/include/cxxopts.hpp"
#include "tree.h"
#include "nary_tree.h"
#include <fstream>

int main(int argc, char** argv) {
    cxxopts::Options options(argv[0], "Produces a tree of specified depth");
    options.positional_help("[output file] [depth]");
    options.add_options("required")
            ("f,file", "output file", cxxopts::value<std::string>())
            ("d,depth", "depth", cxxopts::value<int>());
    options.add_options("optional")
            ("n,nary", "Produce a n-ary tree", cxxopts::value<int>()->default_value("2"))
            ("x,xsep", "Seperation between nodes (x-axis)", cxxopts::value<int>()->default_value("10"))
            ("y,ysep", "Seperation between nodes (y-axis)", cxxopts::value<int>()->default_value("20"))
            ("s,nodesize", "Radius of nodes (in pixels)", cxxopts::value<int>()->default_value("3"))
            ;
    options.parse_positional({"file", "depth"});

    if (argc < 3) {
        std::cout << options.help({"optional"}) << std::endl;
        return 1;
    }

    auto result = options.parse(argc, argv);

    std::string file = result["file"].as<std::string>();
    int depth = result["depth"].as<int>(),
    nodes = result["nary"].as<int>();

    DrawOpts opts = DEFAULT_DRAWING_OPTIONS;
    opts.x_sep = result["xsep"].as<int>();
    opts.y_sep = result["ysep"].as<int>();
    opts.node_size = result["nodesize"].as<int>();

    SVG::SVG tree_drawing;
    if (nodes == 2)
        tree_drawing = draw_binary_tree(depth, opts);
    else
        tree_drawing = draw_nary_tree(nodes, depth, opts);

    std::ofstream outfile(file);
    outfile << tree_drawing.to_string();

    return 0;
}