#include "User.h"

User::User(int id, const char* name) {
	userId=id;
	userName=name;
}
void User::addTask(Task* t) {
	tasks.push_back(t);

}

void User::ShowUserMsg() {
	cout << "\t" << userId << "\t" << userName << "\t" << tasks.size()<<"\n";
}

void User::showTasks() const {
	cout << "====== User "<<userName<<" ======" << endl;
	cout << "Taskid" << "\tPriority" << "\tExecTime" << "\tDescription" << "\tCpuLoad/Device" << endl;
	for (auto& it : tasks) {
		it->printInfo();
	}

}

vector<Task*>& User::getTasks() { return tasks; }

int User::get_id() const { return userId; }

