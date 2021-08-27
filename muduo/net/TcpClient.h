#pragma once
#include "muduo/base/Mutex.h"
#include "muduo/net/TcpConnection.h"

namespace muduo
{
namespace net
{
	class Connector;
	using  ConnectorPtr = std::shared_ptr<Connector>;

	class TcpClient : noncopyable
	{
	private:
		EventLoop* loop_;
		ConnectorPtr connector_; 
		const string name_;
		
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;

		bool retry_;   // atomic
		bool connect_; // atomic
		int nextConnId_;
		mutable MutexLock mutex_;
		TcpConnectionPtr connection_;



		void newConnection(int sockfd);
		void removeConnection(const TcpConnectionPtr& conn);

	public:
		TcpClient(EventLoop* loop,
			const InetAddress& serverAddr,
			const string& nameArg);
		~TcpClient();

		void connect();
		void disconnect();
		void stop();

		TcpConnectionPtr connection() const
		{
			MutexLockGuard lock(mutex_);
			return connection_;
		}

		EventLoop* getLoop() const { return loop_; }
		bool retry() const { return retry_; }
		void enableRetry() { retry_ = true; }
		const string& name() const
		{
			return name_;
		}

		void setConnectionCallback(ConnectionCallback cb)
		{
			connectionCallback_ = std::move(cb);
		}
		void setMessageCallback(MessageCallback cb)
		{
			messageCallback_ = std::move(cb);
		}
		void setWriteCompleteCallback(WriteCompleteCallback cb)
		{
			writeCompleteCallback_ = std::move(cb);
		}
	};
}//net
}//muduo
