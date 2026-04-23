#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "neurochrono/Rate.h"
#include "neurochrono/Timer.h"

namespace {

bool check(bool condition, const char* message) {
	if(!condition) {
		std::cerr << "[FAIL] " << message << "\n";
		return false;
	}
	return true;
}

}

int main() {
	bool ok = true;

	neurochrono::timer_msecs timer;
	std::this_thread::sleep_for(std::chrono::milliseconds(12));
	const auto elapsed = timer.toc<long long>();
	ok &= check(elapsed >= 8, "Timer toc should observe elapsed milliseconds");

	neurochrono::Rate rate(50.0f);
	neurochrono::timer_msecs loop_timer;
	rate.sleep();
	const auto slept = loop_timer.toc<long long>();
	ok &= check(slept >= 15, "Rate sleep should wait close to the requested period");

	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
