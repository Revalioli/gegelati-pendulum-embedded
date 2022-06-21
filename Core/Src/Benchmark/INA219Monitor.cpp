#include "INA219Monitor.h"
#include "ina219.h"
#include <cmath>
#include <iostream>

INA219Monitor::INA219Monitor(INA219_t * ina219t, TIM_HandleTypeDef * tim, bool recordCurrent, bool recordPower)
	: Monitor(tim), currentHistory{0.0}, powerHistory{0}, historyIdx(0),
	  recordCurrent(recordCurrent), recordPower(recordPower), ina219t(ina219t), flushWhenFull(true) {}

void INA219Monitor::clearHistory(){
	this->historyIdx = 0;

	for(int i = 0; i < INA219Monitor::HISTORY_SIZE; i++){
		this->currentHistory[i] = 0.0;
		this->powerHistory[i] = 0.0;
	}
}

void INA219Monitor::flushHistory(){

	for(int i = 0; i < this->historyIdx; i++)
		std::cout << this->currentHistory[i] << '\t' << this->powerHistory[i] << std::endl;

	this->historyIdx = 0;
}

void INA219Monitor::record(){
	this->currentHistory[this->historyIdx] =
		this->recordCurrent ? INA219_ReadCurrent(this->ina219t) : nan("");

	this->powerHistory[this->historyIdx] =
		this->recordPower ? INA219_ReadPower(this->ina219t) : nan("");


	this->historyIdx++;

	if(this->historyIdx == INA219Monitor::HISTORY_SIZE){
		if(this->flushWhenFull)
			flushHistory();
		else
			this->historyIdx = 0;
	}

}


void INA219Monitor::writeHeader(){ std::cout << "Current\tPower" << std::endl; }
