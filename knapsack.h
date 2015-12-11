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

    vector<item> items_packed;      // Items, which are in a knapsack

private:
    vector<item> items_to_choose;   // Items to choose from

    int *y;                         // Temporary solution for recursive version
    int *x;                         // Final solutiong of recursive algo

    // Packs with recursive backtracking
    void pack_rec_backtrack(int i);

    // Packs with dynamic programming
    void pack_dynamic();

    // Packs with greedy algorithm
    void pack_greedy();

    // Packs with iterative backtracking
    void pack_backtrack();

    // Sorts items with bubble sort
    void sort_items();

public:
    // Default constructor
    knapsack() {
        current_price = 0;
    };

    // Constructing a knapsack with a given weight
    knapsack(int max_weight, vector<item> items_to_choose) {
        current_price = 0;
        this->max_weight = max_weight;
        this->items_to_choose = items_to_choose;
    }

    // Destructor
    ~knapsack() {
        items_to_choose.clear();
        items_packed.clear();
    }

    // Enum with packing modes
    enum packing_mode {
        REC_BACKTRACK,
        DYNAMIC,
        GREEDY,
        BACKTRACK
    };

    // Items to choose getter
    const vector<item> &get_items_to_choose() const {
        return items_to_choose;
    }

    // Packs items using specified algorithm
    void pack(packing_mode mode);

    // Prints results to console
    void print_to_file(int index, string filename, double time);

    void print_result(double time);
};

#endif //KNAPSACKPROBLEM_KNAPSACK_H
