#pragma once
#include "muduo/base/Condition.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"

namespace muduo
{
namespace net
{
	class EventLoop;

	class EventLoopThread : noncopyable
	{
	public:
		//intit
		using ThreadInitCallback = std::function<void(EventLoop*)>;
	private:
		EventLoop* loop_;
		bool exiting_;
		Thread thread_;
		MutexLock mutex_;
		Condition cond_;
		ThreadInitCallback callback_;
		void threadFunc();

	public:
		EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
			const string& name = string());
		~EventLoopThread();
		EventLoop* startLoop();
	};
}//net
}//muduo