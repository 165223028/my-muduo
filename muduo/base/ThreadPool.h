#pragma once

#include "muduo/base/Condition.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"
#include "muduo/base/Types.h"

#include <deque>
#include <vector>
namespace muduo
{
	class ThreadPool :noncopyable
	{
	public:
		using Task= std::function<void()>;

		explicit ThreadPool(const string& nameArg = string("ThreadPool"));
		~ThreadPool();

		void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize;}
		void setThreadInitCallback(const Task& cb)
		{
			threadInitCallback_ = cb;
		}

		void start(int numThreads);
		void stop();

		const string& name() const
		{
			return name_;
		}

		size_t queueSize() const;

		void run(Task f);
	private:
		bool isFull() const;
		void runInThread();
		Task take();

		mutable MutexLock mutex_;
		Condition notEmpty_;
		Condition notFull_;

		string name_;
		Task threadInitCallback_;
		std::vector<std::unique_ptr<muduo::Thread>> threads_;
		std::deque<Task> queue_;
		size_t maxQueueSize_;
		bool running_;
	};
}
