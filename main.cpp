#include <iostream>
#include "knapsack.h"
#include "tester.h"

using namespace std;

int main() {

    string file_name = "input_knap.txt";

    tester* backtrack_tester = new tester("Backtrack", 50);
    (*backtrack_tester).start_testing(file_name, knapsack::packing_mode::BACKTRACK);
    (*backtrack_tester).get_csv_stats();
    (*backtrack_tester).print_results();
    (*backtrack_tester).print_results_to_file();
    delete backtrack_tester;

    tester* rec_backtrack_tester = new tester("Recursive backtrack", 50);
    (*rec_backtrack_tester).start_testing(file_name, knapsack::packing_mode::REC_BACKTRACK);
    (*rec_backtrack_tester).get_csv_stats();
    (*rec_backtrack_tester).print_results();
    (*rec_backtrack_tester).print_results_to_file();
    delete rec_backtrack_tester;

    tester* greedy_tester = new tester("Greedy", 50);
    (*greedy_tester).start_testing(file_name, knapsack::packing_mode::GREEDY);
    (*greedy_tester).get_csv_stats();
    (*greedy_tester).print_results();
    (*greedy_tester).print_results_to_file();
    delete greedy_tester;

    tester* dynamic_tester = new tester("Dynamic", 50);
    (*dynamic_tester).start_testing(file_name, knapsack::packing_mode::DYNAMIC);
    (*dynamic_tester).get_csv_stats();
    (*dynamic_tester).print_results();
    (*dynamic_tester).print_results_to_file();
    delete dynamic_tester;

    // TODO: print overall statistics

    return 0;
}