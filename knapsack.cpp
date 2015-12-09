//
// Created by Alex Lazarenko on 03.12.15.
//

#include <iostream>
#include <fstream>
#include "knapsack.h"

/*
 * Packs a knapsack, using simple iterative backtracking
 */
void knapsack::pack_backtrack() {
    int cw = 0, cp = 0;
    for (int k = 0; k <= items_to_choose.size(); k++) {
        if (cw + items_to_choose[k].get_weight() <= max_weight) {
            y[k] = 1;
            if (k < items_to_choose.size()) {
                cp += items_to_choose[k].get_cost();
                cw += items_to_choose[k].get_weight();
                continue;
            }
            if (((cp + items_to_choose[k].get_cost()) > current_price) && (k == items_to_choose.size())) {
                current_price = cp + items_to_choose[k].get_cost();
                current_weight = cw + items_to_choose[k].get_weight();
                for (int j = 0; j < k; j++)
                    x[j] = y[j];
            }
        }
        if (bound(cp, cw, k) >= current_price) {
            y[k] = 0;
            if (k < items_to_choose.size() - 1) {
                continue;
            }
            if ((cp > current_price) && (k == items_to_choose.size() - 1)) {
                current_price = (int) cp;
                current_weight = (int) cw;
                for (int j = 0; j < k; j++)
                    x[j] = y[j];
            }
        }
    }
}

/*
 * Returns the bound for backtracking approach
 *
 * @param: cp - current price
 * @param: cw - current weight
 * @paeam: k - index
 */
float knapsack::bound(float cp, float cw, int k) {
    float b = cp;
    float c = cw;
    for (int i = k; i < items_to_choose.size(); i++) {
        c = c + items_to_choose[i].get_weight();
        if (c <= max_weight)
            b = b + items_to_choose[i].get_cost();
        else
            return (float)(b +
                    ((c - max_weight) / (float) items_to_choose[i].get_weight()) * items_to_choose[i].get_cost());
    }
    return b;
}

/*
 * Packs a knapsack, using backracking with recursion calling
 *
 * To call a function (first time) use (0, 0.0, 0)
 *
 * @param: k - index
 * @param: cp - current price
 * @param: cw - current weight
 */
void knapsack::pack_rec_backtrack(int k, float cp, float cw) {
    if (cw + items_to_choose[k].get_weight() <= max_weight) {
        y[k] = 1;
        if (k < items_to_choose.size() - 1) {
            pack_rec_backtrack(k + 1, cp + items_to_choose[k].get_cost(), cw + items_to_choose[k].get_weight());
        }
        if (((cp + items_to_choose[k].get_cost()) > current_price) && (k == items_to_choose.size() - 1)) {
            current_price = (int) (cp + items_to_choose[k].get_cost());
            current_weight = (int) (cw + items_to_choose[k].get_weight());
            for (int j = 0; j < k; j++)
                x[j] = y[j];
        }
    }
    if (bound(cp, cw, k) >= current_price) {
        y[k] = 0;
        if ((cp > current_price) && (k == items_to_choose.size() - 1)) {
            current_price = (int) cp;
            current_weight = (int) cw;
            for (int j = 0; j < k; j++)
                x[j] = y[j];
        }
        if (k < items_to_choose.size() - 1) {
            pack_rec_backtrack(k + 1, cp, cw);
        }
    }
}


/*
 * Packs current knapsack, using dynamic programming
 */
void knapsack::pack_dynamic() {
    int N = items_to_choose.size();

    double temp1 = clock();
    int **sol = new int *[N + 1];
    for (int i = 0; i <= N; i++) {
        sol[i] = new int[max_weight + 1];
    }
    bad_time = clock() - temp1;

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

    temp1 = clock();
    int w = max_weight;
    for (int n = N; n >= 1; n--) {
        if ((sol[n][w] != sol[n - 1][w])) {
            items_packed.push_back(items_to_choose[n - 1]);
            w -= items_to_choose[n - 1].get_weight();
        }
    }
    bad_time += clock() - temp1;
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
            double y = (double) items_to_choose[j+1].get_cost() / items_to_choose[j+1].get_weight();

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
            sort_items();
            x = new int[items_to_choose.size()];
            y = new int[items_to_choose.size()];
            pack_rec_backtrack(0, 0.0, 0);
            for (int i = 0; i < items_to_choose.size(); i++) {
                if (x[i] == 1) {
                    items_packed.push_back(items_to_choose[i]);
                }
            }
            break;
        case BACKTRACK:
            sort_items();
            x = new int[items_to_choose.size()];
            y = new int[items_to_choose.size()];
            pack_backtrack();
            for (int i = 0; i < items_to_choose.size(); i++) {
                if (x[i] == 1) {
                    items_packed.push_back(items_to_choose[i]);
                }
            }
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


