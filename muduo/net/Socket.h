#pragma once
#include "muduo/base/noncopyable.h"
#include <utility>
//<netinet/tcp.h>
struct tcp_info;

namespace muduo
{
	namespace net
	{
		class InetAddress;

		class Socket : noncopyable
		{
		private:
			const int sockfd_;
		public:
			explicit Socket(int sockfd)
				: sockfd_(sockfd)
			{ }
			explicit Socket(Socket&& soc)
				: sockfd_(soc.sockfd_)
			{}
			~Socket();

			int fd() const { return sockfd_; }
			bool getTcpInfo(struct tcp_info*) const;
			bool getTcpInfoString(char* buf, int len) const;

			void bindAddress(const InetAddress& localaddr);
			void listen();

			int accept(InetAddress* peeraddr);

			void shutdownWrite();
			//TCP_NODELAY
			void setTcpNoDelay(bool on);
			//SO_REUSEADDR
			void setReuseAddr(bool on);
			//SO_REUSEPORT
			void setReusePort(bool on);
			//SO_KEEPALIVE
			void setKeepAlive(bool on);
		};
	}
}
