#pragma once
#include <string>
#include <vector>
#include "Task.h"
using namespace std;
class User {
	int userId;
	string userName;
	vector<Task> tasks;
public:
	User(int id, const char* name);
	void ShowUserMsg();
	void addTask(Task t);
	void showTasks() const;
	int get_id() const;

};
