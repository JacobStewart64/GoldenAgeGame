#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>

namespace ga {
	class sleeper {
		std::timed_mutex mut_;
		std::atomic_bool locked_;
	
		void lock()
		{
			mut_.lock();
			locked_ = true;
		}
	
		void unlock()
		{
			locked_ = false;
			mut_.unlock();
		}

	public:

		sleeper()
		{
			lock();
		}
	
		~sleeper() {
			if (locked_)
			{
				unlock();
			}
		}
	
		template<typename Rep, typename Period >
		void sleepFor(const std::chrono::duration<Rep, Period>& timeout_duration)
		{
			if (mut_.try_lock_for(timeout_duration))
			{
				mut_.unlock();
			}
		}
	
		void wake() {
			if (locked_)
			{
				unlock();
			}
		}
	};
};