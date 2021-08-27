#pragma once
#include "muduo/base/noncopyable.h"
#include "muduo/base/Types.h"

#include <functional>
#include <memory>
#include <vector>

namespace muduo
{
namespace net
{
class EventLoop;
class EventLoopThread;
class EventLoopThreadPool : noncopyable
{
public:
	using ThreadInitCallback = std::function<void(EventLoop*)>;
private:
	EventLoop* baseLoop_;
	string name_;
	bool started_;
	int numThreads_;/*number*/
	int next_;/*next thread*/
	std::vector<std::unique_ptr<EventLoopThread>> threads_;
	std::vector<EventLoop*> loops_;
public:
	EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg);
	~EventLoopThreadPool();

	void setThreadNum(int numThreads) { numThreads_ = numThreads; }
	void start(const ThreadInitCallback& cb = ThreadInitCallback());

	/// round after start
	EventLoop* getNextLoop();

	EventLoop* getLoopForHash(size_t hashCode);

	std::vector<EventLoop*> getAllLoops();

	bool started() const
	{
		return started_;
	}

	const string& name() const
	{
		return name_;
	}
};
}//net
}//muduo
