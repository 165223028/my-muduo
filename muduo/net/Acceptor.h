#pragma once
#include <functional>

#include "muduo/net/Channel.h"
#include "muduo/net/Socket.h"

namespace muduo
{
namespace net
{
	class EventLoop;
	class InetAddress;

	class Acceptor : noncopyable
	{
	public:
		using NewConnectionCallback = std::function<void(int sockfd, const InetAddress&)>;
	private:
		void handleRead();

		EventLoop* loop_;
		Socket acceptSocket_;
		Channel acceptChannel_;
		NewConnectionCallback newConnectionCallback_;
		bool listening_;
		int idleFd_;//temp fd
	public:
		Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
		~Acceptor();

		void setNewConnectionCallback(const NewConnectionCallback& cb)
		{
			newConnectionCallback_ = cb;
		}

		void listen();

		bool listening() const { return listening_; }

	};

}//net
}//muduo