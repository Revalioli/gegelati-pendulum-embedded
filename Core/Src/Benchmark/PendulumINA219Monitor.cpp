#include <PendulumINA219Monitor.h>
#include <iostream>

PendulumINA219Monitor::PendulumINA219Monitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim)
		: INA219Monitor(ina219t, tim), env(env) {}

void PendulumINA219Monitor::record() {

	this->frameRecord[this->historyIdx] = env.getCurrentStep();

	INA219Monitor::record();
}

void PendulumINA219Monitor::flushHistory() {

	for(int i = 0; i < this->historyIdx; i++){
		std::cout << this->frameRecord[i] << '\t'
		<< this->currentHistory[i]  << '\t'
		<< this->powerHistory[i] << std::endl;
	}

}

void PendulumINA219Monitor::writeHeader() { std::cout << "Step\tCurrent\tPower" << std::endl; }
