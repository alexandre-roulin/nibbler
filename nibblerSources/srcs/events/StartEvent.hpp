
#pragma once

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/time_clock.hpp>
#include <boost/date_time.hpp>

struct StartEvent {
	StartEvent(boost::posix_time::ptime st = boost::posix_time::second_clock::local_time());
	boost::posix_time::ptime start_time;
};


