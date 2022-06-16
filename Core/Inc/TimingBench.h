#ifndef SRC_TIMINGBENCH_H_
#define SRC_TIMINGBENCH_H_

#include "main.h"
#include <vector>

class TimingBench {

	/// Timer used for benchmark
	TIM_HandleTypeDef * tim;

	/// Function to time
	void (*bench_fun)(void);

	/// Holds timing results for each attempts
	std::vector<int> attemptResults;

	/// Number of timing test to do
	int nbAttempts;

	/// Last average timing
	int result;

public:
	TimingBench(void (*bench_fun)(void), TIM_HandleTypeDef * timer, int nbAttempts = 5);
	virtual ~TimingBench() {};

	/// Start bench, return the timing (average on results of all attempts)
	/// Multiple this result by the period of the timer to get the actual elapsed time
	int startBench();
};

#endif /* SRC_TIMINGBENCH_H_ */
