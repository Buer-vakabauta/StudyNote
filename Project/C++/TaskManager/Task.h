#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//�������
class Task {
protected:
	int taskId;//����ID
	int priority;//�������ȼ�
	int execTime;//ִ��ʱ��
	string description;//��������
public:
	Task(int id, int priority, int execTime, const char* description);
	int getPriority() const;
	virtual void run();//ִ������
	virtual void printInfo() const;//���������Ϣ
};

//��������
class ComputationTask :public Task {

private:
int cpuLoad;//��Ҫ��CPU��Դ
public:
	ComputationTask(int id, int priority, int execTime, const char* description, int cpuLoad);
	void printInfo() const;
	void run() override;
};

//IO����
class IOtask :public Task {
private:
	string devicename;//�豸����
public:
	IOtask(int id, int priority, int execTime, const char* description, const char* devicename);
	void printInfo() const;
	void run() override;
};

//�滮����
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



