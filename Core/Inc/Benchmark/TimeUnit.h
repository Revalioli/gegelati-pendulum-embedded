#ifndef TIMEUNIT_H_
#define TIMEUNIT_H_

enum class TimeUnit {
	Seconds,
	Milliseconds,
	Microseconds,
	None
};

const char * getTimeUnitString(TimeUnit unit);

#endif
