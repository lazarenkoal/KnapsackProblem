//
// Created by Alex Lazarenko on 03.12.15.
//

#include <fstream>
#include <iostream>
#include "tester.h"

/*
 * Launches testing process for a set of knaps of specified type
 *
 * @param file_name - place to save the result
 * @param mode - algorithm to use
 */
void tester::start_testing(string file_name, knapsack::packing_mode mode) {

    // File readers
    fstream infile;
    infile.open(file_name, ios::in);

    // Read num of tests
    int num_of_tests;
    infile >> num_of_tests;

    // Read experimental data
    for (int i = 0; i < num_of_tests; i++) {

        // Read knap capacity
        int knap_capacity;
        infile >> knap_capacity;

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

        // Create item objects
        for (int j = 0; j < n; j++) {
            items_to_choose.push_back(*(new item(j, costs[j], weights[j])));
        }

        // Create snap and perform experiments
        knapsack *knap = new knapsack();
        double *timeElapsed = new double[number_of_iterations];

        // Perform test with time measurements
        for (int j = 0; j < number_of_iterations; j++) {
            knap = new knapsack(knap_capacity, items_to_choose);
            auto temp1 = std::chrono::steady_clock::now();
            knap->pack(mode);
            auto temp2 = std::chrono::steady_clock::now();

            // Getting time in seconds
            auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(temp2 - temp1);

            // Converting to nanoseconds
            timeElapsed[j] = (double)elapsed_time.count();
        }


        // Count avg time
        double avg_time = 0;
        for (int j = 0; j < number_of_iterations; j++) {
            avg_time += timeElapsed[j];
        }
        avg_time /= number_of_iterations;

        // Free memory
        items_to_choose.clear();

        knaps.push_back(*knap);
        times.push_back(avg_time);
    }

    infile.close();
}

/*
 * Prints results of all knaps in collection
 */
void tester::print_results() {
    cout << type << endl;
    for (int i = 0; i < knaps.size(); i++) {
        knaps[i].print_result(times[i]);
    }
}

/*
 * Prints the results of experiment to the specified file
 * Version that saves the result to separate file
 */
void tester::print_results_to_file() {
    ofstream output_stream;
    string filename = type + "_output.txt";
    output_stream.open(filename);
    output_stream << type << endl;
    output_stream.close();
    for (int i = 0; i < knaps.size(); i++) {
        knaps[i].print_to_file(i, filename, times[i]);
    }
}

/*
 * Prints result to general file
 */
void tester::print_results_to_file(string file_name) {
    ofstream output_stream;
    output_stream.open(file_name, std::ios_base::app);
    output_stream << this->type << endl;
    output_stream.close();
    for (int i = 0; i < knaps.size(); i++) {
        knaps[i].print_to_file(i, file_name, times[i]);
    }
}

/*
 * Creates csv file with stats from experiment
 */
void tester::get_csv_stats() {
    ofstream output_stream;
    output_stream.open(type + "_stats.csv");
    output_stream << "Time,capacity" << endl;
    for (int i = 0; i < knaps.size(); i++) {
        output_stream << knaps[i].get_items_to_choose().size() - 1 << ',' << times[i] << endl;
    }
    output_stream.close();
}

/*
 * Prints the results of
 */
void tester::print_overall_file(tester a, tester b, tester c, tester d) {
    string output_knap = "output_knap.txt";
    a.print_results_to_file(output_knap);
    b.print_results_to_file(output_knap);
    c.print_results_to_file(output_knap);
    d.print_results_to_file(output_knap);
}

/*
 * Launchs testing process for every tester
 *
 * @param input_file - name of input file
 * @param number_of_iterations - amount of iterations to perform for time measurement
 */
void tester::test_samples(string input_file, int number_of_iterations) {

    // Iterative backtrack
    tester *backtrack_tester = new tester("Backtrack", number_of_iterations);
    (*backtrack_tester).start_testing(input_file, knapsack::BACKTRACK);
    (*backtrack_tester).get_csv_stats();
    (*backtrack_tester).print_results();
    (*backtrack_tester).print_results_to_file();

    // Greedy algo
    tester *greedy_tester = new tester("Greedy", number_of_iterations);
    (*greedy_tester).start_testing(input_file, knapsack::GREEDY);
    (*greedy_tester).get_csv_stats();
    (*greedy_tester).print_results();
    (*greedy_tester).print_results_to_file();

    // Dynamic algo
    tester *dynamic_tester = new tester("Dynamic", number_of_iterations);
    (*dynamic_tester).start_testing(input_file, knapsack::DYNAMIC);
    (*dynamic_tester).get_csv_stats();
    (*dynamic_tester).print_results();
    (*dynamic_tester).print_results_to_file();

    // Recursive backtrack
    tester *rec_backtrack_tester = new tester("Recursive backtrack", number_of_iterations);
    (*rec_backtrack_tester).start_testing(input_file, knapsack::REC_BACKTRACK);
    (*rec_backtrack_tester).get_csv_stats();
    (*rec_backtrack_tester).print_results();
    (*rec_backtrack_tester).print_results_to_file();

    // Prints overall result to output_knap.txt
    tester::print_overall_file(*backtrack_tester, *greedy_tester, *dynamic_tester, *rec_backtrack_tester);

    // Clean memory
    delete backtrack_tester;
    delete greedy_tester;
    delete dynamic_tester;
    delete rec_backtrack_tester;
}


