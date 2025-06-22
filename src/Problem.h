#include <iostream>

class Problem {
    protected:
        std::string problem_title; // 題目名稱
        std::string input_path; // 測資: 輸入路徑
        std::string output_path; // 測資: 答案路徑
        int magic_number; // 魔法數字 (下個作業會用到)
    public:
        Problem() = default;
        Problem(std::string title, std::string input, std::string output, int magic_num);
        std::string getTitle() const { return problem_title; }
        std::string getInput() const { return input_path; }
        std::string getOutput() const { return output_path; }
        int getNum() const { return magic_number; }
};
class ProblemSystem : public Problem {
    private:
        std::vector<Problem> problem_list; // 所有題目
        void adding_problem(Problem new_problem); // Function: 將新題目資料更新至對應資料結構與變數
    public:
        void init(std::string &PROBLEM_DATA_PATH); // Function: 初始化 (讀取題目資料)
        std::vector<Problem>* list_problem(); // Function: 列出所有題目
        void newproblem_set(std::string PROBLEM_DATA_PATH); // Function: 新增題目
};