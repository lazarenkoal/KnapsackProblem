//
// Created by Alex Lazarenko on 03.12.15.
//

#ifndef KNAPSACKPROBLEM_ITEM_H
#define KNAPSACKPROBLEM_ITEM_H

/*
 * Class represents an item to be packed
 */
class item {
private:
    int id;             // Number of an item
    int cost;           // Cost if item
    int weight;         // Weight of item


public:
    // Default constructor
    item() { };

    // Constructing an item
    item(int id, int cost, int weight) {
        this->id = id;
        this->cost = cost;
        this->weight = weight;
    }

    int get_num() { return this->id; }

    int get_cost() { return this->cost; }

    int get_weight() { return this->weight; }
};

#endif //KNAPSACKPROBLEM_ITEM_H
