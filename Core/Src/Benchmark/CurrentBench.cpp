#include "CurrentBench.h"

#include "main.h"
#include "ina219.h"

CurrentBench::CurrentBench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim) : Bench(fun), monitor(ina219t, tim) {}

void CurrentBench::startBench() {

	this->state = BenchState::Working;

	this->monitor.clearHistory();

	this->monitor.makeActive();
	(*this->benchFunction)();
	CurrentMonitor::noActiveMonitor();

	this->state = BenchState::Ok;

}
