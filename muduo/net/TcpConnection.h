#pragma once

#include "muduo/base/noncopyable.h"
#include "muduo/base/StringPiece.h"
#include "muduo/base/Types.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/InetAddress.h"

#include <memory>

#include <boost/any.hpp>
//#include < netinet / tcp.h >
struct tcp_info;

namespace muduo
{
namespace net
{
	class Channel;
	class EventLoop;
	class Socket;

	class TcpConnection : noncopyable,
		public std::enable_shared_from_this<TcpConnection>
	{
	private:
		using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
		enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };

		EventLoop* loop_;
		const string name_;
		StateE state_;
		bool reading_;

		std::unique_ptr<Socket> socket_;
		std::unique_ptr<Channel> channel_;
		
		const InetAddress localAddr_;
		const InetAddress peerAddr_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		CloseCallback closeCallback_;

		WriteCompleteCallback writeCompleteCallback_;
		HighWaterMarkCallback highWaterMarkCallback_;

		size_t highWaterMark_;
		Buffer inputBuffer_;
		Buffer outputBuffer_;
		boost::any context_;

		void handleRead(Timestamp receiveTime);
		void handleWrite();
		void handleClose();
		void handleError();

		void sendInLoop(const StringPiece& message);
		void sendInLoop(const void* message, size_t len);
		void shutdownInLoop();
		void forceCloseInLoop();
		void startReadInLoop();
		void stopReadInLoop();

		void setState(StateE s) { state_ = s; }
		const char* stateToString() const;
	public:
		TcpConnection(EventLoop* loop,
			const string& name,
			int sockfd,
			const InetAddress& localAddr,
			const InetAddress& peerAddr);
		~TcpConnection();

		EventLoop* getLoop() const { return loop_; }
		const string& name() const { return name_; }
		const InetAddress& localAddress() const { return localAddr_; }
		const InetAddress& peerAddress() const { return peerAddr_; }
		bool connected() const { return state_ == kConnected; }
		bool disconnected() const { return state_ == kDisconnected; }

		bool getTcpInfo(struct tcp_info*) const;
		string getTcpInfoString() const;

		void send(const void* message, int len);
		void send(const StringPiece& message);
		//swap
		void send(Buffer* message);
		void shutdown();

		void forceClose();
		void forceCloseWithDelay(double seconds);
		void setTcpNoDelay(bool on);

		void startRead();
		void stopRead();
		bool isReading() const { return reading_; };

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

		//callback
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

		void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
		{
			highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark;
		}

		void setCloseCallback(const CloseCallback& cb)
		{
			closeCallback_ = cb;
		}

		//Buffer
		Buffer* inputBuffer()
		{
			return &inputBuffer_;
		}

		Buffer* outputBuffer()
		{
			return &outputBuffer_;
		}
		//new connection
		void connectEstablished();
		//delete connection
		void connectDestroyed();
	};
}//net
}//muduo
