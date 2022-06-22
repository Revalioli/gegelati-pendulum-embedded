#include "TimeUnit.h"

const char * getTimeUnitString(TimeUnit unit){

	switch(unit){
		case(TimeUnit::Seconds) : return "s"; break;
		case(TimeUnit::Milliseconds) : return "ms"; break;
		case(TimeUnit::Microseconds) : return "us"; break;
		case(TimeUnit::None) : return ""; break;
	}
}


