#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//任务基类
class Task {
protected:
	int taskId;//任务ID
	int priority;//任务优先级
	int execTime;//执行时间
	string description;//任务描述
public:
	Task(int id, int priority, int execTime, const char* description);
	int getPriority() const;
	virtual void run();//执行任务
	virtual void printInfo() const;//输出任务信息
};

//计算任务
class ComputationTask :public Task {

private:
int cpuLoad;//需要的CPU资源
public:
	ComputationTask(int id, int priority, int execTime, const char* description, int cpuLoad);
	void printInfo() const;
	void run() override;
};

//IO任务
class IOtask :public Task {
private:
	string devicename;//设备名字
public:
	IOtask(int id, int priority, int execTime, const char* description, const char* devicename);
	void printInfo() const;
	void run() override;
};

//规划任务
class Scheduler {
protected:
	vector<Task*> taskqueue;
public:
	virtual void add(Task* t);
	virtual void schedule()=0;
};

class FCFSScheduler:public Scheduler{
	void schedule() override;
};

class Prioritycheduler :public Scheduler {
	void schedule() override;
};



