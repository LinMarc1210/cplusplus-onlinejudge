#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <random>
#include "Problem.h"

#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

Problem::Problem(std::string title, std::string input, std::string output, int magic_num) {
    problem_title = title;
    input_path = input;
    output_path = output;
    magic_number = magic_num;
}
void ProblemSystem::adding_problem(Problem problem) {
    problem_list.push_back(problem);
}
void ProblemSystem::init(std::string &PROBLEM_DATA_PATH) {
    // TODO: Loading problem data from PROBLEM_DATA_PATH
    std::ifstream file(PROBLEM_DATA_PATH);
    if( !file.is_open() ) {
        std::cerr << "Error\n";
        std::cerr << "Please check your csv file in " << PROBLEM_DATA_PATH << "\n";
        exit(1);
    }

    std::string readline;
    while( getline(file, readline) ) {
        std::string title, input_path, output_path, magic_num;
        char c;
        std::stringstream ss(readline);
        getline(ss, title, ',');
        getline(ss, input_path, ',');
        getline(ss, output_path, ',');
        getline(ss, magic_num, ',');
        int magic = std::stoi(magic_num);

        Problem new_problem(title, input_path, output_path, magic);
        ProblemSystem::adding_problem(new_problem);
    }
}
std::vector<Problem>* ProblemSystem::list_problem() {
    // TODO: return the correct variable
    return &this->problem_list;
}
void ProblemSystem::newproblem_set(std::string PROBLEM_DATA_PATH) {

    std::string input, output, problem;
    int num;
    std::cout << yellow("Now, please input the input path(testcase): ");
    getline(std::cin, input);
    std::cout << yellow("Ok, your input path is " + input + "\n");
    std::cout << yellow("Please input the output path(testcase): ");
    getline(std::cin, output);
    std::cout << yellow("Ok, your output path is " + output + "\n");
    std::cout << yellow("Please input the magic number (for this problem): ");
    std::cin >> num;
    std::cout << yellow("Final! please input the problem name: ");
    std::cin.ignore();
    getline(std::cin, problem);
    std::cout << yellow("Ok, your problem name(title) is " + problem + "\n");

    ProblemSystem::adding_problem(Problem(problem, input, output, num));

    try { // update problem data
        std::ofstream info_out(PROBLEM_DATA_PATH);
        if( !info_out ) {
            throw std::runtime_error("Error: File does not exist - " + PROBLEM_DATA_PATH);
        }

        for( Problem problem : problem_list ) {
            info_out << problem.getTitle() << "," << problem.getInput() << "," << problem.getOutput() << "," << problem.getNum() << "\n";
        }

        info_out.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}
