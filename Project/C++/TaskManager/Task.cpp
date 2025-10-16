#include "Task.h"
using namespace std;
//�������
Task::Task(int id, int pri, int exectime, const char* descri) :taskId(id),priority(pri),execTime(exectime),description(descri){}
void Task::run() {}//ִ������


void Task::printInfo() const{
		cout << taskId<< "\t" << priority << "\t" << execTime << "\t" << description << endl;
	}//���������Ϣ

int Task::getPriority() const{ return priority; }
//-------------------------------------------------------------

ComputationTask::ComputationTask(int id, int priority, int execTime, const char* description, int cpuLoad):Task(id,priority,execTime,description){
	this->cpuLoad = cpuLoad;
}

void ComputationTask::run(){}
void ComputationTask::printInfo() const{
	cout << taskId << "\t" << priority << "\t" << execTime << "\t" << description <<'\t' <<cpuLoad<< endl;
}
//-------------------------------------------------------------
IOtask::IOtask(int id, int priority, int execTime, const char* description, const char* devicename) :Task(id, priority, execTime, description) {
	this->devicename = devicename;
}

void IOtask::printInfo() const {

	cout << taskId << "\t" << priority << "\t" << execTime << "\t" << description << '\t' << devicename << endl;
}

void IOtask::run(){}
//-------------------------------------------------------------

void Scheduler::add(Task* t){
	taskqueue.push_back(t);
}
//-------------------------------------------------------------

void FCFSScheduler::schedule() {
	
}

void Prioritycheduler::schedule() {
	sort(taskqueue.begin(), taskqueue.end(), [](const Task* a, const Task* b) {return a->getPriority() > b->getPriority(); });
}
