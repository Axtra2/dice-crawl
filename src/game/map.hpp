#pragma once

#include <../utils/random.hpp>
#include <ostream>
#include <room.hpp>

#include <optional>
#include <vector>
#include <algorithm>

struct Map {
    std::vector<Room> rooms;
    int32_t num_layers;

    Room* boss_room;
    std::vector<std::vector<Room*>> layers;
};

std::optional<Map> loadMap(const char* filename);


class MapGenerator {
public:
    MapGenerator(int num_layers) : current_id(0) {
        m.num_layers = num_layers;
        m.layers.resize(num_layers);
        generate_graph();
    }

    void generate_graph() {
        generate_node(0);
        m.boss_room = generate_node(m.num_layers);
        traverse(m.layers[0][0]);
    }

    void traverse(Room* node) {
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
            Room* new_node = generate_node(curr_layer + 1);
            m.layers[curr_layer + 1].push_back(new_node);
            connections.push_back(new_node);
            traverse(new_node);
        }

        std::optional<int32_t>* fields[] = {&node->n, &node->e, &node->w};
        std::shuffle(std::begin(fields), std::end(fields), getRNG());
        for (int i = 0; i < connections.size(); i++) {
            *fields[i] = connections[i]->id;
        }
    }

    Room* generate_node(int layer) {
        Room node = generateRoom(7, 7, getRNG());
        node.layer = layer;
        node.id = current_id++;
        m.rooms.emplace_back(node);
        return &m.rooms.back();
    }

    void print_graph(std::ostream& os) const {
        for (int layer = 0; layer < m.num_layers; ++layer) {
            os << "Layer " << layer << ":\n";
            for (const auto& node : m.layers[layer]) {
                os << "  Node " << node->id << " connects to [";
                os << "w: " << node->w.value() << ", n: " << node->n.value() << "e: " << node->e.value();
                os << "]\n";
            }
        }
    }


private:
    Map m;
    int32_t current_id;
};