#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cctype>
#include <climits>
#include <thread>
#include <utility>
#include <random>
#include "JudgeSystem.h"
#include "RandomGenerator.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

void clearWindow() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    return;
}
JudgeSystem::JudgeSystem(std::string userPath, std::string problemPath, std::string msgPath, std::string version) {
    USER_DATA_PATH = userPath;
    PROBLEM_DATA_PATH = problemPath;
    LOGIN_MSG_PATH = msgPath;
    VERSION = version;
    status = "NOT READY";
}
void JudgeSystem::effectLoading(std::string content) {
    static const char* spinner[] = {"|", "/", "-", "\\"};
    for (int i = 0, j = 0; j < 20; j++) { 
        std::cout << "\033[33m" << content << "\033[0m" << spinner[i] << "\r";
        std::flush(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        i = ( i + 1 ) % 4;
    }
    return;
}
void JudgeSystem::loadData() {

    // Step1: Call AccountSystem::init with user data path
    AccountSystem::init(JudgeSystem::USER_DATA_PATH);
    JudgeSystem::effectLoading("Status - Loading user data...");
    std::cout << yellow("Status - Loading user data...") << green("OK!\n");

    // Step2: Call ProblemSystem::init with problem data path
    ProblemSystem::init(JudgeSystem::PROBLEM_DATA_PATH);
    JudgeSystem::effectLoading("Status - Loading problem data...");
    std::cout << yellow("Status - Loading problem data...") << green("OK!\n");

    // Step3: welcome the user
    try {
        std::ifstream inputFile("./data/msg/login.txt");
        if( !inputFile ) {
            throw std::runtime_error("Error: File does not exist - ./data/msg/login.txt");
        }

        std::string line;
        std::cout << "\033[36m";
        while (std::getline(inputFile, line)) {
            std::cout << line;
            std::cout << "\n";
        }
        inputFile.close(); 
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }

    return;
}
void JudgeSystem::judge(char id) {
    if (id == '\0') {
        std::cout << "Please input the problem ID: ";
        std::cin >> id;
    }
    std::vector<Problem> problems = *ProblemSystem::list_problem();
    std::string input = problems[id-'A'].getInput();
    std::string output = problems[id-'A'].getOutput();
    std::string code;
    std::cout << "Please input your code name: ";
    std::cin >> code;

    std::string cmd = "g++ " + code + " -o test 2>/dev/null";
    int check = system(cmd.c_str());
    if (check != 0) {
        std::cerr << red("Judge result: Compiler Error!\n");
        return;
    }
    cmd = "./test < " + input;
    FILE* fp = popen(cmd.c_str(), "r");
    if (!fp) {
        std::cerr << red("Judge result: Compiler Error!\n");
    }

    char buffer[1024];
    std::string result;
    while (fgets(buffer, sizeof(buffer), fp)) {  // fgets: 逐行讀取 fp 內容放到 buffer
        result += buffer;
    }
    fclose(fp);

    std::ifstream ans(output);
    std::string line, correct;
    while (getline(ans, line)) {
        correct += line + "\n";
    }
    
    if (correct == result) {
        std::cout << green("Judge result: Accepted\n");
    }
    else {
        std::cout << red("Judge result: Wrong Answer\n");
    }
    return;
}

void JudgeSystem::randomProblem() {
    std::vector<Problem> problems = *ProblemSystem::list_problem();
    if (problems.empty()) {
        std::cout << red("No problems available.\n");
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, problems.size() - 1);

    // 使用 RandomGenerator 抽題目
    RandomGenerator rng(problems.size() / 2);

    int idx = dist(gen);
    char id = 'A' + idx;
    std::cout << yellow("Random Problem ") << id
              << ": " << problems[idx].getTitle() << "\n";

    // judge
    judge(id);
}

int JudgeSystem::mainPage() {
    if( JudgeSystem::status == "NOT READY" ) {
        // call function loadData
        // update status
        JudgeSystem::loadData();
        status = "USER LOGIN";
        return 0;
    }
    else if( JudgeSystem::status == "USER LOGIN" ) {
        std::pair<bool, std::string> user_info = AccountSystem::login();
        if( user_info.first == false ) return 0;
        JudgeSystem::status = "READY";
        return 0;
    }
    
    MainPage::mainpagePrint();
    int opt = MainPage::operationCheck();
    if (opt == 1) {
        std::cout << green("Username: ") << AccountSystem::getuserLogin() << "\n";
    } else if (opt == 2) {
        std::cout << green("VERSION: ") << JudgeSystem::VERSION << "\n";
    } else if (opt == 3) {
        std::vector<Problem> vec = *ProblemSystem::list_problem();
        for (int i = 0 ; i < vec.size() ; i++) {
            std::cout << "Problem " << (char)('A'+i) << ": " << vec[i].getTitle() << "\n";
        }
    } else if (opt == 4) {
        // TODO: random some problems
        JudgeSystem::randomProblem();
    } else if (opt == 5) {
        JudgeSystem::judge();
        return -1;
    } else if (opt == 6) {
        if (AccountSystem::getuserLogin() != "admin") {
            std::cout << red("You are not admin, you cannot add a new problem!\n");
            return 0;
        }
        ProblemSystem::newproblem_set(JudgeSystem::PROBLEM_DATA_PATH);
    } else if (opt == 7) {
        clearWindow();
        return -1;     // exit
    } else {
        std::cout << "Invalid Operation. Please enter operation again: \n";
    }

    return 0;
}