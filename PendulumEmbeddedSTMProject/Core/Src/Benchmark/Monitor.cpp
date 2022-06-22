#include "Monitor.h"


Monitor* Monitor::activeMonitor = nullptr;

Monitor::Monitor(TIM_HandleTypeDef * tim, TimeUnit timUnit, float timMultiplier) : timer(tim), timerUnit(timUnit), timerMultiplier(timMultiplier) {}

void Monitor::makeActive(){

	// Stop timer doing auto measurement for old active monitor
	if(Monitor::activeMonitor != nullptr && Monitor::activeMonitor->timer != nullptr)
		HAL_TIM_Base_Stop_IT(Monitor::activeMonitor->timer);

	Monitor::activeMonitor = this;

	// Start timer for auto measurement
	if(timer != nullptr)
		HAL_TIM_Base_Start_IT(timer);
}

void Monitor::noActiveMonitor(){
	if(Monitor::activeMonitor != nullptr && Monitor::activeMonitor->timer != nullptr)
		HAL_TIM_Base_Stop_IT(Monitor::activeMonitor->timer);

	Monitor::activeMonitor = nullptr;
}



void recordActiveMonitor(){

	if(Monitor::activeMonitor != nullptr)
		Monitor::activeMonitor->record();

}
