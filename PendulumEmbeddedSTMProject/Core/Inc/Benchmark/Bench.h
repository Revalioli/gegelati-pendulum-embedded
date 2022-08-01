#ifndef SRC_BENCHMARK_BENCH_H_
#define SRC_BENCHMARK_BENCH_H_

/**
 * \brief Enumeration for all states of a Bench.
 * 
 */
enum class BenchState {
	Failed,		/// Last benchmark failed or the bench was not yet started, results are invalid.
	Ok,			/// Last benchmark succeeded, results are valid.
	Working		/// Benchmark is currently running.
};

/**
 * \brief Base class for any type of benchmark. It takes a function pointer of type void fun(void)
 * and do actions before and after calling it.
 *
 * This class is intended to be used for benchmarking like timing, current,
 * or any other characteristics about the execution of a function
 * without really focusing on what the function will return.
 */
class Bench {

protected:
	/// The function used for the benchmark.
	void (*benchFunction)(void);

	/// State of the Bench.
	BenchState state;

public:
	/// Base constructor, set the fucntion fun as the function to benchmark.
	Bench(void (*fun)(void));

	virtual ~Bench() {};

	/// Starts bench
	virtual void startBench() = 0;

	/// Set the function to benchmark.
	void setBenchFunction(void (*fun)(void));
};

#endif /* SRC_BENCHMARK_BENCH_H_ */
