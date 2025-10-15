#include "Task.h"
using namespace std;
//�������
Task::Task(int id, int pri, int exectime, const char* descri) :taskId(id),priority(pri),execTime(exectime),description(descri){}
void Task::run() {}//ִ������
void Task::printInfo() const{
		cout << taskId<< "\t" << priority << "\t" << execTime << "\t" << description << endl;
	}//���������Ϣ


ComputationTask::ComputationTask(int id, int priority, int execTime, const char* description, int cpuLoad):Task(id,priority,execTime,description){
	this->cpuLoad = cpuLoad;
}

void ComputationTask::run(){}

IOtask::IOtask(int id, int priority, int execTime, const char* description, const char* devicename) :Task(id, priority, execTime, description) {
	this->devicename = devicename;
}

void IOtask::run(){}