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
    double bad_time;                // Time wasted for additional operations

    vector<item> items_packed;      // Items, which are in a knapsack

    int *y;
    int *x;

    // Functions related to the iterative backtracking
    void pack_backtrack();

    float bound(float cp, float cw, int k);

    void pack_rec_backtrack(int k, float cp, float cw);

    void pack_dynamic();

    void pack_greedy();

    void sort_items();

public:
    knapsack() {
        bad_time = 0;
        current_price = 0;
    };                   // Default constructor



    // Constructing a knapsack with a given weight
    knapsack(int max_weight, vector<item> items_to_choose) {
        this->max_weight = max_weight;
        this->items_to_choose = items_to_choose;
    }

    // Items to choose from
    vector<item> items_to_choose;

    // Prints results to console
    void print_to_file(int index, string filename, double time);

    void print_result(double time);

    double get_bad_time() {
        return this->bad_time;
    }

    // Enum with packing modes
    enum packing_mode {
        BACKTRACK,     // Перебор с возвратом
        REC_BACKTRACK, // Рекурсивный перебор с возвратом
        DYNAMIC,       // Dynamic programming style
        GREEDY         // Жадный
    };

    void pack(packing_mode mode);
};

#endif //KNAPSACKPROBLEM_KNAPSACK_H
