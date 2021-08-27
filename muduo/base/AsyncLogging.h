#pragma once

#include "muduo/base/BlockingQueue.h"
#include "muduo/base/BoundedBlockingQueue.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"
#include "muduo/base/LogStream.h"

#include <atomic>
#include <vector>

namespace muduo
{

	class AsyncLogging : noncopyable
	{
	public:
		AsyncLogging(const string& basename,
			off_t rollSize,
			int flushInterval = 3);

		~AsyncLogging()
		{
			if (running_)
			{
				stop();
			}
		}

		void append(const char* logline, int len);

		void start()
		{
			running_ = true;
			thread_.start();
			latch_.wait();
		}

		void stop() 
		{
			running_ = false;
			cond_.notify();
			thread_.join();
		}

	private:

		void threadFunc();

		using Buffer = muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer>;
		using BufferVector=std::vector<std::unique_ptr<Buffer>> ;
		using BufferPtr = BufferVector::value_type ;

		const int flushInterval_;
		std::atomic<bool> running_;
		const string basename_;
		const off_t rollSize_;
		muduo::Thread thread_;
		muduo::CountDownLatch latch_;
		muduo::MutexLock mutex_;
		muduo::Condition cond_ ;
		BufferPtr currentBuffer_ ;
		BufferPtr nextBuffer_ ;
		BufferVector buffers_ ;
	};
}//muduo
