//
// Created by Alex Lazarenko on 03.12.15.
//

#ifndef KNAPSACKPROBLEM_TESTER_H
#define KNAPSACKPROBLEM_TESTER_H

#include <string>
#include <vector>
#include "item.h"
#include "knapsack.h"

/*
 * Represents tester class for algorithms.
 * It is used for getting results of different experiments
 * and launching algorithms.
 */
class tester {
private:
    vector<knapsack> knaps;         // Knaps of current tester
    vector<double> times;           // Vector with durations of experiments
    string type;                    // Type (rec, backtr, dyn, greedy)
    int number_of_iterations;       // Amount of iterations
public:
    // Constructs tester
    tester(string type, int number_of_iterations) {
        this->type = type;
        knaps.reserve(0);
        times.reserve(0);
        this->number_of_iterations = number_of_iterations;
    }

    // Destructs tester
    ~tester(){
        knaps.clear();
        times.clear();
    }

    // Tests algorithm
    void start_testing(string file_name, knapsack::packing_mode mode);

    // Prints results to console
    void print_results();

    // Prints results to files
    void print_results_to_file();
    void print_results_to_file(string file_name);

    // Makes csv file with time for plots
    void get_csv_stats();

    // Prints final file (which mentioned in the task)
    static void print_overall_file(tester a, tester b, tester c, tester d);

    // Trigger (launches testing of all algorithms)
    static void test_samples(string input_file, int number_of_iterations);

};

#endif //KNAPSACKPROBLEM_TESTER_H
