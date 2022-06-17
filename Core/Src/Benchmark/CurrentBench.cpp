#include "CurrentBench.h"

#include "main.h"
#include "ina219.h"
#include "CurrentMonitor.h"

CurrentBench::CurrentBench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim) : Bench(fun), destroyMonitor(true) {
	this->monitor = new CurrentMonitor(ina219t, tim);
}

CurrentBench::CurrentBench(void (*fun)(void), CurrentMonitor * monitor) : Bench(fun), monitor(monitor), destroyMonitor(false) {}

CurrentBench::~CurrentBench() {
	if(this->destroyMonitor)
		delete this->monitor;
}

void CurrentBench::startBench() {

	this->state = BenchState::Working;

	this->monitor->clearHistory();

	this->monitor->makeActive();
	(*this->benchFunction)();
	CurrentMonitor::noActiveMonitor();

	this->monitor->flushHistory();

	this->state = BenchState::Ok;

}
