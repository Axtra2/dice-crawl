#pragma once

#include <../utils/random.hpp>
#include <cstddef>
#include <ostream>
#include <room.hpp>

#include <optional>
#include <vector>
#include <algorithm>

struct Map {
    std::vector<Room> rooms;
    size_t num_layers;

    Room* boss_room;
    std::vector<std::vector<Room*>> layers;
};

void print_graph(std::ostream& os, const Map& m);

std::optional<Map> loadMap(const char* filename);


class MapGenerator {
public:
    MapGenerator() : current_id(0) {}

    Map generate_graph(int num_layers) {
        width = 7;
        height = 7;
        current_id = 0;

        Map m;
        m.num_layers = num_layers;
        m.layers = {static_cast<size_t>(num_layers), std::vector<Room*>()};

        generate_node(0, m);
        m.boss_room = generate_node(num_layers, m);
        traverse(m.layers[0][0], m);
        return m;
    }

private:
    void traverse(Room* node, Map& m) {
        int32_t curr_layer = node->layer;
        if (curr_layer + 1 >= m.num_layers) {
            return;
        }

        auto gen = getRNG();
        std::uniform_int_distribution<> dis(1, 3);

        int num_nodes = dis(gen);
        std::vector<Room*> connections;

        if (m.layers[curr_layer + 1].size() >= 3) {
            std::uniform_real_distribution<> prob(0.0, 1.0);
            double p = 0.4;
            int old_count = std::count_if(m.layers[curr_layer + 1].begin(), m.layers[curr_layer + 1].end(), 
                [&prob, &gen, p](Room* n) { return prob(gen) > p; });

            if (old_count > num_nodes) old_count = num_nodes;
            std::sample(m.layers[curr_layer + 1].begin(), m.layers[curr_layer + 1].end(), 
                        std::back_inserter(connections), old_count, gen);
            num_nodes -= old_count;
        }

        for (int i = 0; i < num_nodes; ++i) {
            Room* new_node = generate_node(curr_layer + 1, m);
            m.layers[curr_layer + 1].push_back(new_node);
            connections.push_back(new_node);
            traverse(new_node, m);
        }

        std::optional<int32_t>* fields[] = {&node->n, &node->e, &node->w};
        std::shuffle(std::begin(fields), std::end(fields), getRNG());
        for (int i = 0; i < connections.size(); i++) {
            *fields[i] = connections[i]->id;
        }
    }

    Room* generate_node(int layer, Map& m) {
        Room node = generateRoom(width, height, getRNG());
        node.layer = layer;
        node.id = current_id++;
        m.rooms.emplace_back(node);
        return &m.rooms.back();
    }

    int32_t current_id = 0;
    int32_t width = 1;
    int32_t height = 1;
};