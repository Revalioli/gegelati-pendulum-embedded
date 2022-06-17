#include "PendulumCurrentMonitor.h"

#include <iostream>

PendulumCurrentMonitor::PendulumCurrentMonitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim)
		: CurrentMonitor(ina219t, tim), env(env) {}

void PendulumCurrentMonitor::recordCurrent() {

	this->frameRecord[this->currentHistoryIdx] = env.getCurrentStep();

	CurrentMonitor::recordCurrent();
}

void PendulumCurrentMonitor::flushHistory() {

	for(int i = 0; i < this->currentHistoryIdx; i++)
		std::cout << "Step : " << this->frameRecord[i] << ", Current : " << this->currentHistory[i] << std::endl;

}
