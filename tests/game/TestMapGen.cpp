
#include <algorithm>
#include <cassert>
#include <iostream>

#include "map.hpp"

int main() {
    MapGenerator mg;
    Map m = mg.generate_graph(7);
    print_graph(std::cout, m);
}