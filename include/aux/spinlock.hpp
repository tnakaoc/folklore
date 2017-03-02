#pragma once
#include <atomic>
namespace folklore
{
	struct spin_lock
	{
		std::atomic<bool> flag{false};
		void lock(void)
		{
			while(flag.exchange(true)){}
		}
		void unlock(void)
		{
			flag.store(false);
		}
		explicit operator bool() const
		{
			return flag.load();
		}
	};
}
