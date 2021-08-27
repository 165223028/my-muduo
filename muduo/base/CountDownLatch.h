#pragma once
#include "muduo/base/Condition.h"
#include "muduo/base/Mutex.h"

namespace muduo
{
class CountDownLatch : noncopyable
{
	private:
			mutable MutexLock mutex_;
			Condition condition_;
			int count_ ;
	public:
		explicit CountDownLatch(int count);


		void wait();

		void countDown();

		int getCount() const;

};
}