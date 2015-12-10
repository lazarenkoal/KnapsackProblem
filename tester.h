//
// Created by Alex Lazarenko on 03.12.15.
//

#ifndef KNAPSACKPROBLEM_TESTER_H
#define KNAPSACKPROBLEM_TESTER_H

#include <string>
#include <vector>
#include "item.h"
#include "knapsack.h"


class tester {
private:
    vector<knapsack> knaps;
    vector<double> times;
    string type;
    int number_of_iterations;
public:
    tester(string type, int number_of_iterations) {
        this->type = type;
        knaps.reserve(0);
        times.reserve(0);
        this->number_of_iterations = number_of_iterations;
    }

    void start_testing(string file_name, knapsack::packing_mode mode);

    void print_results();

    void print_results_to_file();
    
    void print_results_to_file(string file_name);

    void get_csv_stats();

    static void print_overall_file(tester a, tester b, tester c, tester d);

    static void test_samples(string input_file, int number_of_iterations);
    
    ~tester(){
        knaps.clear();
        times.clear();
    }
};



#endif //KNAPSACKPROBLEM_TESTER_H
