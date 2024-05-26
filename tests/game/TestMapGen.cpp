
#include <algorithm>
#include <cassert>
#include <iostream>

#include "map.hpp"

int main() {
    MapGenerator mg(5);
    mg.print_graph(std::cout);
}