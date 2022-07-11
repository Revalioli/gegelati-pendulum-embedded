#include <PendulumINA219Monitor.h>
#include <iostream>

PendulumINA219Monitor::PendulumINA219Monitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim, TimeUnit timUnit, float timMultiplier)
		: INA219Monitor(ina219t, tim, timUnit, timMultiplier), env(env) {}

void PendulumINA219Monitor::record() {

	this->frameRecord[this->historyIdx] = env.getCurrentStep();

	INA219Monitor::record();
}

void PendulumINA219Monitor::flushHistory() {
	this->skipRecord = 2;

	for(int i = 0; i < this->historyIdx; i++){
		std::cout << this->frameRecord[i] << '\t'
		<< this->currentHistory[i]  << '\t'
		<< this->powerHistory[i] << std::endl;
	}

	this->historyIdx = 0;
}

void PendulumINA219Monitor::writeHeader() {
	std::cout << "dataTimeUnit : " << getTimeUnitString(this->timerUnit)
			<< "\tdataTimerMultiplier : " << this->timerMultiplier
			<< "\tStartAngle : " << this->env.getAngle()			// Environment state before calling the bench function
			<< "\tStartVelocity : " << this->env.getVelocity()
			<< std::endl;
	std::cout << "Step\tCurrent\tPower" << std::endl;
}
