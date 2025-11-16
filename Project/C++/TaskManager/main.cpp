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
	int userindex = -1;
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
		IOtask* task=new IOtask(1, 1, 100, "Print","Printer");
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
		{
			users[userindex - 1].showTasks();
			this->userindex = userindex - 1;
		}
	}
	//显示提示信息
	void ShowTips() {
		cout << "========================================================" << endl;
		if (CuuentSystemState == MainMenu) {
			cout << "1.Input Serial to manage the user's task" << "\n";
			cout << "2.Input 'add userID userName' to add a user" << endl;
			cout << "3.input 'del serial' to delete a user" << endl;
			cout << "Input 'q' to exit" << endl;
		}
		else if (CuuentSystemState == UserMenu) {
			cout << "1. Input 'addtask cpu/io taskId priority execTime description [extra]' add task" << endl;
			cout << "2. Input 'run index' run task" << endl;
			cout << "3. Input 'deltask index' del task" << endl;
			cout << "4. Input 'back' back to mainmenu" << endl;
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
				CuuentSystemState = UserMenu;
				ShowTips();
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
		else if (CuuentSystemState == UserMenu) {
			istringstream iss(s);
			string cmd;
			iss >> cmd;

			if (cmd == "addtask") {
				string type;
				int id, priority, time;
				string description;

				iss >> type >> id >> priority >> time >> description;

				if (type == "io") {
					string device;
					iss >> device;
					IOtask* task=new IOtask(id, priority, time, description.c_str(), device.c_str());

					users[userindex].addTask(task);
					cout << " IO task added。" << endl;
				}
				else if (type == "cpu") {
					int cpuLoad;
					iss >> cpuLoad;
					ComputationTask* task=new ComputationTask(id, priority, time, description.c_str(), cpuLoad);
					users[userindex].addTask(task);
					cout << " cpu task added。" << endl;
				}
				else {
					cout << " unknown type，should be io or cpu。" << endl;
				}
				users[userindex].showTasks();
			}
			else if (cmd == "run") {
				int index;
				if (iss >> index) {
					auto& tasks = users[userindex].getTasks();
					if (index > 0 && index <= tasks.size()) {
						cout << "  Task Running：" << endl;
						tasks[index - 1]->run();
					}
					else {
						cout << " Serial error。" << endl;
					}
				}
			}
			else if (cmd == "deltask") {
				int index;
				if (iss >> index) {
					auto& tasks = users[userindex].getTasks();
					if (index > 0 && index <= tasks.size()) {
						tasks.erase(tasks.begin() + index - 1);
						cout << " task deleted。" << endl;
					}
					else {
						cout << " Serial error。" << endl;
					}
				}
			}
			else if (cmd == "back") {
				CuuentSystemState = MainMenu;
				ShowUsers();
				ShowTips();
			}
			else {
				cout << " input invaild，input addtask/run/deltask/back。" << endl;
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