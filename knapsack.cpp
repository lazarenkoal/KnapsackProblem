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

    vector<int> *visited = new vector<int>();
    vector<int> *best = new vector<int>();

    bool have_finished = false;
    do {
        // если можно дабавить элемент
        if ((j != items_to_choose.size()) && (current_weight + items_to_choose[j].get_weight() <= max_weight)) {
            current_weight += items_to_choose[j].get_weight();
            current_price += items_to_choose[j].get_cost();
            visited -> push_back(j);
            j++;
        }
        else {
            //если остались не просмотренные решения
            if (j < items_to_choose.size() - 1) {
                j++;
            }
            else {
                //если это решение лучше
                bool is_best_price_bigger = best_price < current_price;
                bool is_best_price_equal = best_price == current_price;
                bool is_best_weight_bigger = best_weight > current_weight;

                if ( is_best_price_bigger || (is_best_price_equal && is_best_weight_bigger)) {
                    best -> clear();
                    best -> insert(best -> begin(), visited -> begin(), visited -> end());
                    best_price = current_price;
                    best_weight = current_weight;
                }
                if (visited -> size() == 0) {
                    have_finished = true;
                    continue;
                }
                if ((*visited)[visited -> size() - 1] + 1 < items_to_choose.size()) {
                    current_weight -= items_to_choose[(*visited)[visited -> size() - 1]].get_weight();
                    current_price -= items_to_choose[(*visited)[visited -> size() - 1]].get_cost();
                    j = (*visited)[visited -> size() - 1] + 1;
                    visited -> pop_back();
                } else {
                    if (visited -> size() == 1) {
                        have_finished = true;
                    } else {
                        if (visited -> size() == 0) {
                            have_finished = true;
                            continue;
                        }
                        current_weight -= items_to_choose[(*visited)[visited -> size() - 1]].get_weight();
                        current_price -= items_to_choose[(*visited)[visited -> size() - 1]].get_cost();
                        visited -> pop_back();
                        current_weight -= items_to_choose[(*visited)[visited -> size() - 1]].get_weight();
                        current_price -= items_to_choose[(*visited)[visited -> size() - 1]].get_cost();
                        j = (*visited)[visited -> size() - 1] + 1;
                        visited -> pop_back();
                    }

                }
            }
        }
    } while (!have_finished);

    for (int i = 0; i < best -> size(); i++) {
        items_packed.push_back(items_to_choose[(*best)[i]]);
    }

    delete visited;
    delete best;
    return;
}

/*
 * Packs a knapsack, using backracking with recursion calling
 *
 * To call a function (first time) use (0)
 *
 * @param: i - index
 */
void knapsack::pack_rec_backtrack(int i) {

    // Initializing the result of the algorithm
    if (i > items_to_choose.size()) {
        if (current_price > best_price) {
            best_price = current_price;
            for (int j = 0; j < items_to_choose.size(); j++) {
                x[j] = y[j];
            }
        }
        return;
    }

    // Searching the right sub-tree
    if (current_weight + items_to_choose[i].get_weight() <= max_weight) {
        current_weight += items_to_choose[i].get_weight();
        current_price += items_to_choose[i].get_cost();
        y[i] = 1;
        pack_rec_backtrack(i + 1);
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
    int N = (int)items_to_choose.size();

    //double temp1 = clock();
    int **sol = new int *[N + 1];
    for (int i = 0; i <= N; i++) {
        sol[i] = new int[max_weight + 1];
    }
    //bad_time = clock() - temp1;

    for (int i = 0; i <= N; i++) {
        for (int w = 0; w <= max_weight; w++) {
            if (i == 0 || w == 0) {
                sol[i][w] = 0;
            } else if (items_to_choose[i - 1].get_weight() <= w) {
                sol[i][w] = max(items_to_choose[i - 1].get_cost()
                                + sol[i - 1][w - items_to_choose[i - 1].get_weight()],
                                sol[i - 1][w]);
            } else {
                sol[i][w] = sol[i - 1][w];
            }
        }
    }

    //temp1 = clock();
    int w = max_weight;
    for (int n = N; n >= 1; n--) {
        if ((sol[n][w] != sol[n - 1][w])) {
            items_packed.push_back(items_to_choose[n - 1]);
            w -= items_to_choose[n - 1].get_weight();
        }
    }
    //bad_time += clock() - temp1;
}

/*
 * Packs current knapsack using greedy algorithm
 */
void knapsack::pack_greedy() {
    sort_items();
    long size = items_to_choose.size();
    int total = max_weight;
    for (int j = 0; j < size; j++) {
        if (items_to_choose[j].get_weight() <= total) {
            items_packed.push_back(items_to_choose[j]);
            total -= items_to_choose[j].get_weight();
        } else {
            break;
        }
    }
}

/*
 * Sorts elements so that price[i]/weight[i] >= p[i+1]/w[i+1]
 */
void knapsack::sort_items() {
    long size = items_to_choose.size();
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
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
            x = new int[items_to_choose.size()];
            y = new int[items_to_choose.size()];
            pack_rec_backtrack(0);
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


