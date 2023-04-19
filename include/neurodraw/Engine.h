#ifndef NEURODRAW_ENGINE_H_
#define NEURODRAW_ENGINE_H_

#include <chrono>
#include <mutex>
#include <thread>

namespace neurodraw {

class Engine {

	public:	
		virtual ~Engine(void);

	protected:
		Engine(void);

		virtual void run(void) = 0;
		void start(void);
		void stop(void);
		void teardown(void);
		void join(void);
		bool is_running(void);
		void lock(void);
		void unlock(void);

		std::string name(void);

	protected:
		std::string name_;

	private:
		bool run_;
		std::thread thread_;
		std::mutex mutex_;


};

}


#endif
