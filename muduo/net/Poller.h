#pragma once
#include <map>
#include <vector>

#include "muduo/base/Timestamp.h"
#include "muduo/net/EventLoop.h"

namespace muduo
{
	namespace net
	{
		class Channel;

		class Poller : noncopyable
		{
		private:
			EventLoop* ownerLoop_;
		protected:
			using  ChannelMap = std::map<int, Channel*>;
			ChannelMap channels_;

		public:
			using ChannelList = std::vector<Channel*>;

			Poller(EventLoop* loop);
			virtual ~Poller();

			virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

			virtual void updateChannel(Channel* channel) = 0;

			virtual void removeChannel(Channel* channel) = 0;

			virtual bool hasChannel(Channel* channel) const;

			static Poller* newDefaultPoller(EventLoop* loop);

			void assertInLoopThread() const
			{
				ownerLoop_->assertInLoopThread();
			}
		};
	}//net
}//muduo
