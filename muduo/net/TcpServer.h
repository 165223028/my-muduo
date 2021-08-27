#pragma once
#include "muduo/base/Atomic.h"
#include "muduo/base/Types.h"
#include "muduo/net/TcpConnection.h"

#include <map>

namespace muduo
{
namespace net
{
	class Acceptor;
	class EventLoop;
	class EventLoopThreadPool;

	//single-threaded or thread-pool models
	//interface class
	class TcpServer : noncopyable
	{
	public:
		using ThreadInitCallback = std::function<void(EventLoop*)>;
		enum Option
		{
			kNoReusePort,
			kReusePort,
		};
	private:
		using ConnectionMap = std::map<string, TcpConnectionPtr>;

		EventLoop* loop_;
		const string ipPort_;
		const string name_;
		std::unique_ptr<Acceptor> acceptor_;
		std::shared_ptr<EventLoopThreadPool> threadPool_;


		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;
		ThreadInitCallback threadInitCallback_;

		AtomicInt32 started_;
		int nextConnId_;/*con Num*/
		ConnectionMap connections_;/*con Map*/

		void newConnection(int sockfd, const InetAddress& peerAddr);
		void removeConnection(const TcpConnectionPtr& conn);
		void removeConnectionInLoop(const TcpConnectionPtr& conn);

	public:
		TcpServer(EventLoop* loop,
			const InetAddress& listenAddr,
			const string& nameArg,
			Option option = kNoReusePort);
		~TcpServer();

		const string& ipPort() const { return ipPort_; }
		const string& name() const { return name_; }
		EventLoop* getLoop() const { return loop_; }
		//set threads num
		void setThreadNum(int numThreads);
		void setThreadInitCallback(const ThreadInitCallback& cb)
		{
			threadInitCallback_ = cb;
		}
		std::shared_ptr<EventLoopThreadPool> threadPool()
		{
			return threadPool_;
		}
		void start();

		void setConnectionCallback(const ConnectionCallback& cb)
		{
			connectionCallback_ = cb;
		}
		void setMessageCallback(const MessageCallback& cb)
		{
			messageCallback_ = cb;
		}
		void setWriteCompleteCallback(const WriteCompleteCallback& cb)
		{
			writeCompleteCallback_ = cb;
		}
	};
}//net
}//muduo
