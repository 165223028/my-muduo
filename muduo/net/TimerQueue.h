#pragma once
#include <set>
#include <vector>

#include "muduo/base/Mutex.h"
#include "muduo/base/Timestamp.h"

#include "muduo/net/Callbacks.h"
#include "muduo/net/Channel.h"

namespace muduo
{
	namespace net
	{
		class EventLoop;
		class Timer;
		class TimerId;

		//usually useful
		class TimerQueue : noncopyable
		{
		private:
			using Entry = std::pair<Timestamp, Timer*>;
			using TimerList = std::set<Entry>;
			using ActiveTimer = std::pair<Timer*, int64_t>;
			using ActiveTimerSet = std::set<ActiveTimer>;

			void addTimerInLoop(Timer* timer);
			void cancelInLoop(TimerId timerId);

			void handleRead();

			std::vector<Entry> getExpired(Timestamp now);
			void reset(const std::vector<Entry>& expired, Timestamp now);

			bool insert(Timer* timer);

			EventLoop* loop_;
			const int timerfd_;
			Channel timerfdChannel_;
			//sort by time
			TimerList timers_;//set

			//sort by address
			ActiveTimerSet activeTimers_;
			bool callingExpiredTimers_; /* atomic */

			ActiveTimerSet cancelingTimers_;
		public:
			explicit TimerQueue(EventLoop* loop);
			~TimerQueue();

			//repeats if  interval > 0.0.
			TimerId addTimer(TimerCallback cb,
				Timestamp when,
				double interval);

			void cancel(TimerId timerId);
		};
	}//net
}//muduo