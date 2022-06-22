#include "INA219Bench.h"

#include "INA219Monitor.h"

INA219Bench::INA219Bench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim,
		TimeUnit timUnit, float timMultiplier, bool recordCurrent, bool recordPower)
			: Bench(fun), destroyMonitor(true)
{
	this->monitor = new INA219Monitor(ina219t, tim, timUnit, timMultiplier, recordCurrent, recordPower);
}

INA219Bench::INA219Bench(void (*fun)(void),  INA219Monitor * monitor)
		: Bench(fun), monitor(monitor), destroyMonitor(false) {}

INA219Bench::~INA219Bench() {
	if(this->destroyMonitor)
		delete this->monitor;
}

void INA219Bench::startBench(){

	this->state = BenchState::Working;

	this->monitor->clearHistory();

	this->monitor->flushWhenFull = true;

	this->monitor->writeHeader();
	this->monitor->makeActive();
	(*this->benchFunction)();
	INA219Monitor::noActiveMonitor();

	this->monitor->flushHistory();

	this->state = BenchState::Ok;

}
