#include "Wrapper.h"

void Timer::start_measuring()
{
	this->start_time_ = std::chrono::system_clock::now();
}

double Timer::stop_measuring()
{
	this->end_time_ = std::chrono::system_clock::now();
	return (this->end_time_ - this->start_time_).count();
}
