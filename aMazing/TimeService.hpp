#pragma once

#include <chrono>
#include <memory>
#include <iostream>
#include <functional>
#include <thread>
#include <list>
using namespace std;

class TimeService {
public:
	TimeService() {
		isRunning = true;
		processor.reset(new std::thread([&]{
			while(isRunning || hasTask()) {
			if(!hasTask()) {
				continue ;
			}
			std::size_t nowTime = getTickCount();
			for(auto it = tasks.begin();it != tasks.end(); ++it) {
				auto& task = *it;
				if(nowTime - task.startTime >= task.duration) {
					task.task();
					tasks.erase(it);
				}
			}
			}
		}));
	}
	~TimeService() {
		isRunning = false;
		processor->join();
	}
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> timePoint;
	static std::size_t getTickCount(){
		auto nowTime = std::chrono::high_resolution_clock::now();
		auto duration = nowTime - initTime.value;
		return duration.count();
	}
	static timePoint getSysTime() noexcept{
		return chrono::high_resolution_clock::now();
	}
	
	void preCallBack(std::function<void()> n,std::size_t uSec) {
		singleTask tk;
		tk.startTime = getTickCount();
		tk.duration = uSec;
		tk.task = n;
		tasks.push_back(tk);
	}

	bool hasTask() const noexcept {
		return !tasks.empty();
	}
private:
	struct singleTask {
		std::size_t duration;
		std::size_t startTime;
		std::function<void()> task;
	};
	struct time_wrapper { 
		timePoint value; 
		time_wrapper() noexcept{ 
			value = std::chrono::high_resolution_clock::now();
		} 
	};
	std::unique_ptr<std::thread> processor;
	std::list<singleTask > tasks;
	bool isRunning;
	const static time_wrapper initTime;
};
/*
int main() {
	TimeService ser;
	ser.preCallBack([]{std::cout<<"testHahaha"<<std::endl;},10000000000);
	std::cout<< ser.getTickCount();
	return 0;
}
*/
