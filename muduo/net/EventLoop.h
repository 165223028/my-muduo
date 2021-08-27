#pragma once
#include <atomic>
#include <functional>
#include <vector>

#include <boost/any.hpp>

#include "muduo/base/Mutex.h"
#include "muduo/base/CurrentThread.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/TimerId.h"

namespace muduo
{
	namespace net
	{
		class Channel;
		class Poller;
		class TimerQueue;

		class EventLoop :noncopyable
		{
		public:
			using Functor = std::function<void()> ;
		private:
			using ChannelList = std::vector<Channel*>;
			//flag
			bool looping_; /* atomic */
			std::atomic<bool> quit_;
			bool eventHandling_; /* atomic */
			bool callingPendingFunctors_; /* atomic */


			int64_t iteration_;//loop num
			const pid_t threadId_;
			Timestamp pollReturnTime_;
			std::unique_ptr<Poller> poller_;
			std::unique_ptr<TimerQueue> timerQueue_;
			int wakeupFd_;
			std::unique_ptr<Channel> wakeupChannel_;
			boost::any context_;

			ChannelList activeChannels_;
			Channel* currentActiveChannel_;

			mutable MutexLock mutex_;
			std::vector<Functor> pendingFunctors_;

			void abortNotInLoopThread();
			void handleRead();  // waked up
			void doPendingFunctors();

			void printActiveChannels() const; // DEBUG
		public:
			EventLoop();
			~EventLoop();

			void loop();
			void quit();

			Timestamp pollReturnTime() const { return pollReturnTime_; }
			int64_t iteration() const { return iteration_; }
			//run in local loop
			void runInLoop(Functor cb);
			void queueInLoop(Functor cb);
			size_t queueSize() const;

			//timers
			TimerId runAt(Timestamp time, TimerCallback cb);
			TimerId runAfter(double delay, TimerCallback cb);
			TimerId runEvery(double interval, TimerCallback cb);
			void cancel(TimerId timerId);

			//internal
			void wakeup();
			void updateChannel(Channel* channel);
			void removeChannel(Channel* channel);
			bool hasChannel(Channel* channel);

			//pid
			void assertInLoopThread()
			{
				if (!isInLoopThread())
				{
					abortNotInLoopThread();
				}
			}
			bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
			bool eventHandling() const { return eventHandling_; }

			//context
			void setContext(const boost::any& context)
			{
				context_ = context;
			}
			const boost::any& getContext() const
			{
				return context_;
			}
			boost::any* getMutableContext()
			{
				return &context_;
			}

			static EventLoop* getEventLoopOfCurrentThread();
		};
	}//net
}//muduo
