#ifndef NEUROCHORNO_RATE_H_
#define NEUROCHORNO_RATE_H_

#include <thread>
#include <chrono>
#include <cmath>
#include "neurochrono/Timer.h"

namespace neurochrono {


class Rate {

	public:
		Rate(float rate);
		~Rate(void);


		void sleep(void);

	private:
		int msecs_;
		timer_msecs timer_;

};


}


#endif
