#ifndef NEUROCHORNO_TIMER_H_
#define NEUROCHORNO_TIMER_H_

#include <chrono>

namespace neurochrono {

using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

template<typename _Clock, typename _Dur>
using time_point = std::chrono::time_point<_Clock, _Dur>;

using default_clock_t = std::conditional< std::chrono::high_resolution_clock::is_steady,
                                          std::chrono::high_resolution_clock,
                                          std::chrono::steady_clock >::type;

/**
 * @brief Timer. A tic-toc timer.
 *
 * Mesure the elapsed time between construction - or tic() -
 * and toc(). The elapsed time is expressed in unit.
 *
 * @param unit. The time unit.
 * @see unit
 */
template <typename unit>
class Timer {
	public:
	
		/**
  		 * @brief Timer. Launch the timer.
  		 */
  		Timer(): start_(now()) { }

  		/**
  		 * @brief ~Timer. Default desctructor.
  		 */
  		~Timer() = default;

  		/**
  		 * @brief tic. Reset the timer.
  		 */
  		void tic() {
			start_ = now();
  		}

  		/**
  		 * @brief toc. Return this elapsed time since construction or last tic().
  		 * @return double. The elapsed time.
  		 * @see tic()
  		 */
  		template <typename T = int64_t>
  		T toc() {
			return static_cast<T>(cast_d(now() - start_).count());
  		}

	protected:

		time_point<default_clock_t, unit> start_;

  		template <typename... Args>
  		auto cast_d(Args&&... args) ->
			decltype(std::chrono::duration_cast<unit>(std::forward<Args>(args)...)) {
			return std::chrono::duration_cast<unit>(std::forward<Args>(args)...);
  		}

  		template <typename... Args>
  		auto cast(Args&&... args) ->
			decltype(std::chrono::time_point_cast<unit>(std::forward<Args>(args)...)) {
			return std::chrono::time_point_cast<unit>(std::forward<Args>(args)...);
  		}

  		auto now() ->
  		decltype(std::declval<Timer<unit>>().cast(default_clock_t::now()))
  		{
  		  return cast(default_clock_t::now());
  		}
};

using timer_secs  = Timer<seconds>;
using timer_msecs = Timer<milliseconds>;
using timer_usecs = Timer<microseconds>;
using timer_nsecs = Timer<nanoseconds>;

}



#endif
