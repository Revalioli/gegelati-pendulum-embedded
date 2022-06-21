#include "PendulumCurrentMonitor.h"

#include <iostream>

PendulumCurrentMonitor::PendulumCurrentMonitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim)
		: INA219Monitor(ina219t, tim), env(env) {}

void PendulumCurrentMonitor::record() {

	this->frameRecord[this->historyIdx] = env.getCurrentStep();

	INA219Monitor::record();
}

void PendulumCurrentMonitor::flushHistory() {

	for(int i = 0; i < this->historyIdx; i++){
		std::cout << this->frameRecord[i] << '\t'
		<< this->currentHistory[i]  << '\t'
		<< this->powerHistory[i] << std::endl;
	}

}

void PendulumCurrentMonitor::writeHeader() { std::cout << "Step\tCurrent\tPower" << std::endl; }
