//
// Created by Alex Lazarenko on 03.12.15.
//

#ifndef KNAPSACKPROBLEM_KNAPSACK_H
#define KNAPSACKPROBLEM_KNAPSACK_H

#include "item.h"
#include "vector"

using namespace std;

/*
 * Class represents a knapsack to be packed
 */
class knapsack {

private:
    int max_weight;                 // Max allowed weight of a particular knapsack
    int current_price;              // Current price of this knapsack
    int current_weight;             // Current weight of a knapsack
    int best_price;                 // Best price of the pack
    double bad_time;                // Time wasted for additional operations

    vector<item> items_packed;      // Items, which are in a knapsack

    int *y;
    int *x;

    // Functions related to the iterative backtracking


    void pack_rec_backtrack(int i);

    void pack_dynamic();

    void pack_greedy();

    void sort_items();

public:
    knapsack() {
        bad_time = 0;
        current_price = 0;
        visited_nodes.reserve(1);
        best_pack.reserve(1);
    };                   // Default constructor



    // Constructing a knapsack with a given weight
    knapsack(int max_weight, vector<item> items_to_choose) {
        bad_time = 0;
        current_price = 0;
        visited_nodes.reserve(1);
        best_pack.reserve(1);
        this->max_weight = max_weight;
        this->items_to_choose = items_to_choose;
    }

    ~knapsack(){
        items_to_choose.clear();
        visited_nodes.clear();
        best_pack.clear();
        items_packed.clear();
    }

    void pack_backtrack();

    // Items to choose from
    vector<item> items_to_choose;

    vector<int> visited_nodes;
    vector<int> best_pack;

    // Prints results to console
    void print_to_file(int index, string filename, double time);

    void print_result(double time);

    // Enum with packing modes
    enum packing_mode {
        REC_BACKTRACK, // Рекурсивный перебор с возвратом
        DYNAMIC,       // Dynamic programming style
        GREEDY,
        BACKTRACK// Жадный
    };

    void pack(packing_mode mode);
};

#endif //KNAPSACKPROBLEM_KNAPSACK_H
