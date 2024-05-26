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
    size_t num_layers = 0;

    int32_t boss_room = -1;
    std::vector<std::vector<int32_t>> layers;
};

void print_graph(std::ostream& os, const Map& m);

std::optional<Map> loadMap(const char* filename);


class MapGenerator {
public:
    MapGenerator() = default;

    Map generate_graph(int num_layers) {
        width = 7;
        height = 7;
        current_id = 0;

        Map m;
        m.num_layers = num_layers;
        m.layers = {static_cast<size_t>(num_layers + 1), std::vector<int32_t>()};

        generate_node(0, m);
        m.boss_room = generate_node(num_layers, m);
        traverse(m.layers[0][0], m);
        return m;
    }

private:
    void traverse(int32_t node_idx, Map& m) {
        int32_t num_nodes;
        int32_t curr_layer;
        std::vector<int32_t> connections;
        {
            auto node = m.rooms.at(node_idx);
            curr_layer = node.layer;
            if (curr_layer + 1 >= m.num_layers) {
                m.rooms.at(node_idx).n = m.boss_room;
                return;
            }

            auto& gen = getRNG();
            std::uniform_int_distribution<> dis(1, 3);

            num_nodes = dis(gen);

            if (m.layers[curr_layer + 1].size() >= 3) {
                std::uniform_real_distribution<> prob(0.0, 1.0);
                double p = 0.4;
                int old_count = std::count_if(m.layers[curr_layer + 1].begin(), m.layers[curr_layer + 1].end(),
                    [&prob, &gen, p](int32_t n) { return prob(gen) > p; });

                if (old_count > num_nodes) old_count = num_nodes;
                std::sample(m.layers[curr_layer + 1].begin(), m.layers[curr_layer + 1].end(),
                            std::back_inserter(connections), old_count, gen);
                num_nodes -= old_count;
            }
        }
        for (int i = 0; i < num_nodes; ++i) {
            int32_t new_node = generate_node(curr_layer + 1, m);
            connections.push_back(new_node);
            traverse(new_node, m);
        }


        auto& node = m.rooms.at(node_idx);
        std::optional<int32_t>* fields[] = {&node.n, &node.e, &node.w};
        std::shuffle(std::begin(fields), std::end(fields), getRNG());
        for (int i = 0; i < connections.size(); i++) {
            *fields[i] = connections[i];
        }
    }

    int32_t generate_node(int layer, Map& m) {
        Room node = generateRoom(width, height, getRNG());
        node.layer = layer;
        node.id = current_id++;
        m.rooms.emplace_back(node);
        m.layers[layer].emplace_back(node.id);
        return node.id;
    }

    int32_t current_id = 0;
    int32_t width = 1;
    int32_t height = 1;
};