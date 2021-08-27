#pragma once
#include "muduo/base/Timestamp.h"

#include <functional>
#include <memory>

namespace muduo
{

	using std::placeholders::_1;
	using std::placeholders::_2;
	using std::placeholders::_3;

	template<typename T>
	inline T* get_pointer(const std::shared_ptr<T>& ptr)
	{
		return ptr.get();
	}

	template<typename T>
	inline T* get_pointer(const std::unique_ptr<T>& ptr)
	{
		return ptr.get();
	}

	template<typename To, typename From>
	inline ::std::shared_ptr<To> down_pointer_cast(const ::std::shared_ptr<From>& f) {
		if (false)
		{
			implicit_cast<From*, To*>(0);
		}
		return ::std::static_pointer_cast<To>(f);
	}

	namespace net 
	{
		//client visible callbacks.
		class Buffer;
		class TcpConnection;

		using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
		using TimerCallback = std::function<void()>;
		using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
		using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
		using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
		using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr&, size_t)>;

		// the data read to (buf, len)
		using MessageCallback = std::function<void(const TcpConnectionPtr&,
			Buffer*, Timestamp)>;

		void defaultConnectionCallback(const TcpConnectionPtr& conn);
		void defaultMessageCallback(const TcpConnectionPtr& conn,
			Buffer* buffer,
			Timestamp receiveTime);
	}

}//muduo