#include <iostream>
#include <vector>
#include <utility>
#include "User.h"

class AccountSystem : private User {
    private:
        std::vector<User> user_list; // 所有使用者資料
        std::string login_user; // 目前登入者
        std::string USER_DATA_PATH; // 使用者資料儲存位置
        void sign_up(); // Function: 註冊
        void userdataUpdate(); // Function: 更新 USER_DATA_PATH 內的資料
    public:
        void init(std::string USER_DATA_PATH); // Function: 初始化 (讀取資料)
        User* search(std::string name); // Function: 查詢 name 是否存在
        std::pair<bool, std::string> login(); // Function: 登入
        void adding_user(std::string name, std::string password); // Function：新增使用者 name，並將其密碼設定為 password
        std::string getuserLogin(); // Function: 獲取目前登入的使用者是誰
};