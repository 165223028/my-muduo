#pragma once
#include "muduo/net/Poller.h"

#include <vector>

struct epoll_event;

namespace muduo
{
	namespace net
	{
		class EPollPoller : public Poller
		{
		private:
			using EventList = std::vector<struct epoll_event>;

			int epollfd_;
			EventList events_;

			static const int kInitEventListSize = 16;
			
			static const char* operationToString(int op);

			void fillActiveChannels(int numEvents,
				ChannelList* activeChannels) const;
			
			void update(int operation, Channel* channel);

		public:
			EPollPoller(EventLoop* loop);
			~EPollPoller() override;

			Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
			void updateChannel(Channel* channel) override;
			void removeChannel(Channel* channel) override;

		};
	}//net
}//muduo