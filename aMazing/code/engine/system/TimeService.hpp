#pragma once

#include <chrono>
#include <memory>
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
			auto nowTime = std::chrono::high_resolution_clock::now();
			for(auto it = tasks.begin();it != tasks.end(); ++it) {
				auto& task = *it;
				if((nowTime - task.startTime) >= task.duration) {
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
	static timePoint getSysTime() _NOEXCEPT{
		return chrono::high_resolution_clock::now();
	}
	
	void preCallBackMinutes(std::function<void()> n,std::size_t min) {
		preCallBackPrototype<std::chrono::minutes> (n,min);
	}
	void preCallBackSeconds(std::function<void()> n,std::size_t sec) {
		preCallBackPrototype<std::chrono::seconds> (n,sec);
	}
	void preCallBackMilliSec(std::function<void()> n,std::size_t milliSec) {
		preCallBackPrototype<std::chrono::milliseconds> (n,milliSec);
	}
	void preCallBackNanoSec(std::function<void()> n,std::size_t nanoSec) {
		preCallBackPrototype<std::chrono::nanoseconds> (n,nanoSec);
	}
	bool hasTask() const _NOEXCEPT{
		return !tasks.empty();
	}
private:
	template<typename C>
	void preCallBackPrototype(std::function<void()> n,std::size_t duration) {
		singleTask tk;
		tk.startTime = std::chrono::high_resolution_clock::now();
		tk.duration = C(duration);
		tk.task = n;
		tasks.push_back(tk);
	}
	struct singleTask {
		std::chrono::nanoseconds duration;
		timePoint startTime;
		std::function<void()> task;
	};
	struct time_wrapper { 
		timePoint value; 
		time_wrapper() _NOEXCEPT{ 
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
	ser.preCallBackSeconds([]{std::cout<<"testHahaha"<<std::endl;},1);
	std::cout<< ser.getTickCount();
	return 0;
}
*/
