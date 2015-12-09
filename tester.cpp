//
// Created by Alex Lazarenko on 03.12.15.
//

#include <fstream>
#include <iostream>
#include "tester.h"
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;


void tester::start_testing(string file_name, knapsack::packing_mode mode) {

    // File readers
    fstream infile;
    infile.open(file_name, ios::in);

    // Read num of tests
    int num_of_tests;
    infile >> num_of_tests;

    // Read knap capacity
    int knap_capacity;
    infile >> knap_capacity;

    // Read experimental data
    for (int i = 0; i < num_of_tests; i++) {

        // Read test number
        int n;
        infile >> n;

        // Read weights
        int *weights = new int[n];
        for (int j = 0; j < n; j++) {
            infile >> weights[j];
        }

        // Read costs
        int *costs = new int[n];
        for (int j = 0; j < n; j++) {
            infile >> costs[j];
        }

        // Set items to choose vector
        vector<item> items_to_choose;
        items_to_choose.push_back(*(new item(-1, 0, 0)));

        // Create item objects
        for (int j = 0; j < n; j++) {
            items_to_choose.push_back(*(new item(j, costs[j], weights[j])));
        }

        // Create snap and perform experiments
        knapsack *knap = new knapsack();
        double *timeElapsed = new double[number_of_iterations];

        for (int i = 0; i < number_of_iterations; i++) {
            knap = new knapsack(knap_capacity, items_to_choose);
            double temp1 = clock();
            knap->pack(mode);
            double temp2 = clock();
            //double elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(temp2 - temp1).count();
            double elapsed_time = temp2 - temp1 - knap->get_bad_time();
            timeElapsed[i] = elapsed_time;
        }


        // Count avg time
        double avg_time = 0;
        for (int i = 0; i < number_of_iterations; i++) {
            avg_time += timeElapsed[i];
        }
        avg_time /= number_of_iterations;

        knaps.push_back(*knap);
        times.push_back(avg_time);
    }
}

void tester::print_results() {
    cout << type << endl;
    for (int i = 0; i < knaps.size(); i++){
        knaps[i].print_result(times[i]);
    }
}

/*
 * Prints the results of experiment to the specified file
 */
void tester::print_results_to_file() {
    ofstream output_stream;
    string filename = type + "_output.txt";
    output_stream.open(filename);
    output_stream << type << endl;
    output_stream.close();
    for (int i = 0; i < knaps.size(); i++){
        knaps[i].print_to_file(i, filename, times[i]);
    }
}

/*
 * Creates csv file with stats from experiment
 */
void tester::get_csv_stats() {
    ofstream output_stream;
    output_stream.open(type + "_stats.csv");
    output_stream << "Time,capacity" << endl;
    for (int i = 0; i < knaps.size(); i++){
        output_stream << knaps[i].items_to_choose.size() - 1  << ',' << times[i] << endl;
    }
    output_stream.close();
}

void tester::print_overall_file(tester a, tester b, tester c, tester d) {

}
