#include "tree.h"
#include <chrono>
#include <iostream>

int main() {
    for (int i = 10; i < 20; i++) {
        TreeNode tree;
        binary_tree(&tree, i);
        auto start = std::chrono::system_clock::now();
        tree.calculate_xy();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << i << " " << diff.count() << std::endl;
    }
    
    return 0;
}