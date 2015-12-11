//
// Created by Alex Lazarenko on 03.12.15.
//

#include <iostream>
#include <fstream>
#include "knapsack.h"
#include <thread>

/*
 * Packs a knapsack, using simple iterative backtracking
 */
void knapsack::pack_backtrack() {
    int j = 0;

    int best_price = 0;
    int best_weight = 0;

    vector<int> *visited_nodes = new vector<int>();
    vector<int> *best_pack = new vector<int>();

    int amount_of_items_to_pack = (int) items_to_choose.size();

    bool have_finished = false;
    do {
        // If we can add an element to the knapsack -> just go and add
        bool have_not_checked_every_item = j != amount_of_items_to_pack;
        bool can_add_more_weight = current_weight + items_to_choose[j].get_weight() <= max_weight;
        if (have_not_checked_every_item && can_add_more_weight) {
            current_weight += items_to_choose[j].get_weight();
            current_price += items_to_choose[j].get_cost();
            visited_nodes->push_back(j);
            j++;
        }
        else {
            // If we still have something to check
            bool have_smth_to_check = j < amount_of_items_to_pack - 1;
            if (have_smth_to_check) {
                j++;
            }
            else {
                // If this solution is the best of all
                bool is_best_price_bigger = best_price < current_price;
                bool is_best_price_equal = best_price == current_price;
                bool is_best_weight_bigger = best_weight > current_weight;
                if (is_best_price_bigger || (is_best_price_equal && is_best_weight_bigger)) {
                    best_pack->clear();
                    best_pack->insert(best_pack->begin(), visited_nodes->begin(), visited_nodes->end());
                    best_price = current_price;
                    best_weight = current_weight;
                }
                // Reached optimum
                if (visited_nodes->size() == 0) {
                    have_finished = true;
                    continue;
                }
                bool can_look_deeper = (*visited_nodes)[visited_nodes->size() - 1] + 1 < amount_of_items_to_pack;
                if (can_look_deeper) {
                    current_weight -= items_to_choose[(*visited_nodes)[visited_nodes->size() - 1]].get_weight();
                    current_price -= items_to_choose[(*visited_nodes)[visited_nodes->size() - 1]].get_cost();
                    j = (*visited_nodes)[visited_nodes->size() - 1] + 1;
                    visited_nodes->pop_back();
                } else {
                    // Reached optimum
                    if (visited_nodes->size() == 1) {
                        have_finished = true;
                    } else {
                        // Reached optimum
                        if (visited_nodes->size() == 0) {
                            have_finished = true;
                            continue;
                        }
                        // Decrease everything, remove from visited nodes
                        current_weight -= items_to_choose[(*visited_nodes)[visited_nodes->size() - 1]].get_weight();
                        current_price -= items_to_choose[(*visited_nodes)[visited_nodes->size() - 1]].get_cost();
                        visited_nodes->pop_back();
                        current_weight -= items_to_choose[(*visited_nodes)[visited_nodes->size() - 1]].get_weight();
                        current_price -= items_to_choose[(*visited_nodes)[visited_nodes->size() - 1]].get_cost();
                        j = (*visited_nodes)[visited_nodes->size() - 1] + 1;
                        visited_nodes->pop_back();
                    }

                }
            }
        }
    } while (!have_finished);

    // Get optimum knapsack
    for (int i = 0; i < best_pack->size(); i++) {
        items_packed.push_back(items_to_choose[(*best_pack)[i]]);
    }

    // Clean memory
    delete visited_nodes;
    delete best_pack;

    return;
}

/*
 * Packs a knapsack, using backtracking with recursion calling
 *
 * To call a function (first time) use (0)
 *
 * @param: i - index (actually, depth of recursion)
 */
void knapsack::pack_rec_backtrack(int i) {

    int amount_of_items = (int) items_to_choose.size();

    /*
     * y - contains solution for a problem
     * x - saving our solution
     */
    if (i > amount_of_items) {
        if (current_price > best_price) {
            best_price = current_price;
            for (int j = 0; j < amount_of_items; j++) {
                x[j] = y[j];
            }
        }
        return;
    }

    // Searching the right sub-tree
    bool can_add_more_weight = current_weight + items_to_choose[i].get_weight() <= max_weight;
    if (can_add_more_weight) {
        current_weight += items_to_choose[i].get_weight();
        current_price += items_to_choose[i].get_cost();
        y[i] = 1;
        pack_rec_backtrack(i + 1);

        // Restore before going to another tree
        current_weight -= items_to_choose[i].get_weight();
        current_price -= items_to_choose[i].get_cost();
    }
    y[i] = 0;

    // Searching left sub-tree
    pack_rec_backtrack(i + 1);
}


/*
 * Packs current knapsack, using dynamic programming
 */
void knapsack::pack_dynamic() {
    int N = (int) items_to_choose.size();

    int **solution = new int *[N + 1];
    for (int i = 0; i <= N; i++) {
        solution[i] = new int[max_weight + 1];
    }

    for (int i = 0; i <= N; i++) {
        for (int w = 0; w <= max_weight; w++) {
            if (i == 0 || w == 0) {
                solution[i][w] = 0;
            } else if (items_to_choose[i - 1].get_weight() <= w) {
                solution[i][w] = max(items_to_choose[i - 1].get_cost()
                                     + solution[i - 1][w - items_to_choose[i - 1].get_weight()],
                                     solution[i - 1][w]);
            } else {
                solution[i][w] = solution[i - 1][w];
            }
        }
    }

    // Gets out final solution
    int w = max_weight;
    for (int n = N; n >= 1; n--) {
        if ((solution[n][w] != solution[n - 1][w])) {
            items_packed.push_back(items_to_choose[n - 1]);
            w -= items_to_choose[n - 1].get_weight();
        }
    }
}

/*
 * Packs current knapsack using greedy algorithm
 */
void knapsack::pack_greedy() {

    // Before launching the algorithm we need to sort it
    sort_items();
    long size = items_to_choose.size();
    int total = max_weight;
    for (int j = 0; j < size; j++) {
        // Put in knapsack, if it is not too heavy
        if (items_to_choose[j].get_weight() <= total) {
            items_packed.push_back(items_to_choose[j]);
            total -= items_to_choose[j].get_weight();
        } else {
            // Reached limit
            break;
        }
    }
}

/*
 * Sorts elements so that price[i]/weight[i] >= p[i+1]/w[i+1]
 * Simple bubble sort. Why not? Greedy will be the best (time) even with stupid sort
 */
void knapsack::sort_items() {
    long size = items_to_choose.size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            // price / weight
            double x = (double) items_to_choose[j].get_cost() / items_to_choose[j].get_weight();
            double y = (double) items_to_choose[j + 1].get_cost() / items_to_choose[j + 1].get_weight();

            if (x < y) {
                item item1 = items_to_choose[j + 1];
                items_to_choose[j + 1] = items_to_choose[j];
                items_to_choose[j] = item1;
            }
        }
    }
}


/*
 * Public method for packing knapsack, using specified
 * method.
 *
 * @param: mode - mode, which will be used for packing
 */
void knapsack::pack(knapsack::packing_mode mode) {
    switch (mode) {
        case DYNAMIC:
            pack_dynamic();
            break;
        case GREEDY:
            pack_greedy();
            break;
        case REC_BACKTRACK:
            // Initialising everything before going to recursion
            x = new int[items_to_choose.size()];
            y = new int[items_to_choose.size()];
            pack_rec_backtrack(0);
            // Getting out the solution
            for (int i = 0; i < items_to_choose.size(); i++) {
                if (x[i] == 1) {
                    items_packed.push_back(items_to_choose[i]);
                }
            }
            break;
        case BACKTRACK:
            pack_backtrack();
            break;
        default:
            break;
    }
}

/*
 * Function prints the result of packing
 *
 * @param: index - number of experiment
 * @param: filename - file name to print to
 * @param: time - elapsed time of packing
 */
void knapsack::print_to_file(int index, string filename, double time) {
    ofstream output;
    output.open(filename, std::ios_base::app);
    output << "Experiment # " << index << endl;
    output << "The contents of the knapsack: " << endl;
    current_weight = 0;
    current_price = 0;
    for (int i = 0; i < items_packed.size(); i++) {
        output
        //<< "ID: " << items_packed[i].get_num() << ' '
        << "Weight: " << items_packed[i].get_weight() << ' '
        << "Price: " << items_packed[i].get_cost() << '\n';

        current_price += items_packed[i].get_cost();
        current_weight += items_packed[i].get_weight();
    }

    output << "Max weight: " << this->max_weight << '\n';
    output << "Price: " << this->current_price << '\n';
    output << "Total weight: " << this->current_weight << '\n';
    output << "Elapsed time: ";
    output << time << '\n';
    output << endl;
    output.close();
}

/*
 * Function prints the result of packing
 *
 * @param: time - elapsed time of packing
 */
void knapsack::print_result(double time) {
    current_weight = 0;
    current_price = 0;
    for (int i = 0; i < items_packed.size(); i++) {

        cout
        << items_packed[i].get_num() << ' '
        << items_packed[i].get_weight() << ' '
        << items_packed[i].get_cost() << endl;

        current_price += items_packed[i].get_cost();
        current_weight += items_packed[i].get_weight();
    }

    cout << "Max weight: " << this->max_weight << endl;
    cout << "Price: " << this->current_price << endl;
    cout << "Total weight: " << this->current_weight << endl;
    cout << "Elapsed time: " << time << endl;
    cout << endl;
}


