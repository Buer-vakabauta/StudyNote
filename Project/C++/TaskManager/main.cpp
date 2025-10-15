#include "Task.h"
#include "User.h"
#include <sstream>
bool isnum(const string& s) {
	if (s.empty()) return false;
	for (char c : s) {
		if (!isdigit(c)) return false;

	}
	return true;
}



class SystemManager {
private:
	vector<User> users;
	Scheduler* scheduler;
	//系统状态,表示当前界面位置
	enum SystemState {
		MainMenu,
		UserMenu,
		TaskMenu
	};
	SystemState CuuentSystemState;
	//判断是否有该用户
	bool isexist(int userID) {
		for (auto& it : users) {
			if (userID == it.get_id()) return true;
		}
		return false;
	}

public:
	SystemManager() {
		CuuentSystemState = MainMenu;
		User user(10086,"admin");
		Task task(1, 1, 100, "计算");
		user.addTask(task);
		users.push_back(user);
	}
	void ShowUsers() {
		int c = 0;
		cout << "Serial" << "\tUserID" << "\tUserName" << "\tTaskcount" << endl;
		for (auto& it : users) {
			cout << ++c;
			it.ShowUserMsg();
		}
	}
	void ShowUserTask(int userindex) {
		if (userindex > 0 && userindex <= users.size())
			users[userindex - 1].showTasks();
	}
	//显示提示信息
	void ShowTips() {
		if (CuuentSystemState == MainMenu) {
			cout << "========================================================"<<endl;
			cout << "1.Input Serial to manage the user's task" << "\n";
			cout << "2.Input 'add userID userName' to add a user" << endl;
			cout << "3.input 'del serial' to delete a user" << endl;
			cout << "Input 'q' to exit" << endl;
		}

	}
	//根据当前界面来解析用户输入


	void ParsingInput(string& s) {
		if (CuuentSystemState == MainMenu) {
			if (isnum(s))
			{
				int num = stoi(s);
				if (num < 0 && num > users.size()) { cout << "Wrong Serial Number!\n"; return; }
				ShowUserTask(stoi(s));
				return;
			}
			else {
				istringstream iss(s);
				string cmd;
				iss >> cmd;
				if (cmd == "add") {
					int userId;
					string userName;
					if(iss>>userId>>userName){
						if (isexist(userId)) { cout << "the User has existed!!!" << endl; return; }
						else { User user(userId, userName.c_str()); users.push_back(user); cout << "The User has created."<<endl; ShowUsers(); return; }
					 }
					else { cout << "error input!!!"<<endl; return; }

				}
				else if(cmd=="del"){
					int index;
					if (iss >> index) { if (index<0 && index>users.size()) { cout << "Serial out of range" << endl; return; }users.erase(users.begin() + index - 1); cout << "Selected User has delete." << endl; }
					else{ cout << "error input!!!" << endl; return; }
				}
				else { cout << "vaild cmd!!!" << endl; return; }
			}
		}
	}


};

int main() {
	string operate;
	SystemManager mainSystem;
	cout << "====== Task Management System ======"<<endl;
	mainSystem.ShowUsers();
	mainSystem.ShowTips();


	while (1)
	{
		getline(cin,operate);
		if (operate == "q") { cout << "+++++++++++++END++++++++++++++++++"; return 1; }
		mainSystem.ParsingInput(operate);
	}
}