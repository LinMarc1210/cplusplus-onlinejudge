#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>
#include "AccountSystem.h"
#define red(text)    "\033[31m" text "\033[0m"
#define green(text)  "\033[32m" text "\033[0m"
#define yellow(text) "\033[33m" text "\033[0m"
#define blue(text)   "\033[34m" text "\033[0m"
#define magenta(text) "\033[35m" text "\033[0m"
#define cyan(text)   "\033[36m" text "\033[0m"
#define white(text)  "\033[37m" text "\033[0m"

User* AccountSystem::search(std::string name) {
    for (size_t i = 0 ; i < user_list.size() ; i++) {
        // do something 
        if (user_list[i].getUsername() == name) {
            return &user_list[i];
        }
    }
    return nullptr;
}
void AccountSystem::init(std::string USER_DATA_PATH) {

    AccountSystem::USER_DATA_PATH = USER_DATA_PATH;

    try {

        std::ifstream file(USER_DATA_PATH); // 開啟檔案
        if( !file ) {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        // TODO: Loading user data from USER_DATA_PATH and call adding_user (from AccountSystem) function to insert data
        // Hints: stringstream
        std::string read_line;
        while( getline(file, read_line) ) {
            std::string username, password;
            
            // do something
            std::stringstream ss(read_line);
            std::getline(ss, username, ',');
            std::getline(ss, password, ',');

            AccountSystem::adding_user(username, password);
        }
        file.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}
void AccountSystem::sign_up() {
    std::string user;
    std::cout << yellow("Welcome! please enter your name: ");
    getline(std::cin, user);

    while (true) {
        std::string pwd1, pwd2;
        std::cout << yellow("Please enter your password: ");
        getline(std::cin, pwd1);
        std::cout << yellow("Please enter your password again: ");
        getline(std::cin, pwd2);
        if (pwd1 != pwd2) {
            std::cout << red("two passwords are not the same, please try again") << "\n";
        } else {
            AccountSystem::adding_user(user, pwd1);
            break;
        }
    }

    return;
}
std::pair<bool, std::string> AccountSystem::login() {
    // this function will be called until status == "USER LOGIN"
    std::pair<bool, std::string> result = std::make_pair(false, "");
    
    std::string user;
    while (true) {
        std::cout << yellow("User Name (Enter -1 to sign up): ");
        getline(std::cin, user);
        if (user == "-1") {
            AccountSystem::sign_up();
        }
        else {
            if (AccountSystem::search(user)) {
                std::cout << "Welcome aboard, " << user << ".\n";

                std::string pwd;
                std::cout << "Please enter your password: ";
                getline(std::cin, pwd);
                User *cur_user = AccountSystem::search(user);

                int wrong = 0;
                while (wrong < 3) {
                    if (pwd == cur_user->getPassword()) {
                        std::cout << green("Login Success!!! welcome aboard") << "\n";
                        AccountSystem::login_user = user;
                        result.first = true;
                        result.second = user;
                        return result;
                    }
                    wrong++;
                    if (wrong < 3) {
                        std::cout << red("Password incorrect... please try again: ");
                        getline(std::cin, pwd);
                    } else {
                        std::cout << red("Too many unsuccessful sign-in attempts...") << "\n";
                    }
                }
            } else {
                std::cout << red("User is not exist!") << "\n";
            }
        }
    }
    return result;
}
void AccountSystem::adding_user(std::string username, std::string password) {
    User new_user(username, password);
    user_list.push_back(new_user);
    AccountSystem::userdataUpdate();
    return;
}
std::string AccountSystem::getuserLogin() {
    return AccountSystem::login_user;
}
void AccountSystem::userdataUpdate() {
    try {
        std::ofstream info_out(AccountSystem::USER_DATA_PATH);
        if( !info_out ) {
            throw std::runtime_error("Error: File does not exist - " + USER_DATA_PATH);
        }

        // 把整個 vector 跑過一次，透過 info_out 寫入
        // Hints: 你可能會需要 class User 的 getter 才能取得某些資訊
        for (auto user : user_list) {
            info_out << user.getUsername() << "," << user.getPassword() << "\n";
        }

        info_out.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
}