#include "TimingBench.h"

#include <algorithm>
#include <iostream>
#include "main.h"

TimingBench::TimingBench(void (*fun)(void), TIM_HandleTypeDef * timer, int nbAttempts, TimeUnit unit, float unitMultiplier)
		: Bench(fun), tim(timer), timerUnit(unit), timerMultiplier(unitMultiplier), nbAttempts(nbAttempts), result(-1)
{
	HAL_TIM_Base_Stop(timer);
}

void TimingBench::startBench(){

	this->state = BenchState::Working;

	// Reset results
	this->result = 0;
	this->attemptResults.clear();

	for(int i = 0; i < this->nbAttempts; i++){
		// Reset counter and start timer
		__HAL_TIM_SET_COUNTER(this->tim, (int32_t)(0x0));
		HAL_TIM_Base_Start(this->tim);

		(*this->benchFunction)();

		HAL_TIM_Base_Stop(tim);
		uint32_t raw = (uint32_t)(__HAL_TIM_GET_COUNTER(this->tim));
		int timing = (int)raw;
		this->attemptResults.push_back(timing);
		this->result += timing;
	}

	// Compute final result
	this->result = this->result / this->nbAttempts;

	this->state = BenchState::Ok;
}

int TimingBench::getResult() { return this->result; }


void TimingBench::printResult(){

	if(this->state == BenchState::Ok){

		int maxTime = *std::max_element(this->attemptResults.cbegin(), this->attemptResults.cend());
		int minTime = *std::min_element(this->attemptResults.cbegin(), this->attemptResults.cend());

		// Choose unit string
		char * unitStr;

		switch(this->timerUnit){
			case(TimeUnit::Seconds) : unitStr = "s"; break;
			case(TimeUnit::Milliseconds) : unitStr = "ms"; break;
			case(TimeUnit::Microseconds) : unitStr = "us"; break;
		}


		std::cout << "***** Last TimingBench results *****" << std::endl;

		std::cout << "=== T_min : " << (minTime * this->timerMultiplier) << ' ' << unitStr << std::endl;
		std::cout << "=== T_avg : " << (this->result * this->timerMultiplier) << ' ' << unitStr << std::endl;
		std::cout << "=== T_max : " << (maxTime * this->timerMultiplier) << ' ' << unitStr << std::endl;

	}

}



