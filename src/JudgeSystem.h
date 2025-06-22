#include <iostream>
#include "AccountSystem.h"
#include "Problem.h"
#include "MainPage.h"
class JudgeSystem : private AccountSystem, private ProblemSystem, private MainPage {
    private:
        std::string USER_DATA_PATH; // 讀取 user 資料的路徑
        std::string PROBLEM_DATA_PATH; // 讀取題目資料的路徑
        std::string LOGIN_MSG_PATH; // 讀取訊息的路徑
        std::string VERSION; // 版本
        std::string status; // 記錄目前狀態 (初始化？未登入？已登入？)
        void loadData(); // 讀取使用者資料 (初始化)
        void effectLoading(std::string content); // 就是特效 :P
    public:
        JudgeSystem() = default;
        JudgeSystem(std::string userPath, std::string problemPath, std::string msgPath, std::string version);
        void initSystem(); // Function: 初始化系統
        int mainPage(); // Function: 主頁面 (選單)
        void judge(char id = '\0'); // 編譯使用者的答案
        void randomProblem(); // 編譯使用者的答案
        std::string getUserpath() const { return USER_DATA_PATH; }
        std::string getProblempath() const { return PROBLEM_DATA_PATH; }
        std::string getMsgpath() const { return LOGIN_MSG_PATH; }
        std::string getVersion() const { return VERSION; }
};