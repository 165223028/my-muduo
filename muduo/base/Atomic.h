#pragma once
#include "muduo/base/noncopyable.h"
#include <atomic>

namespace muduo 
{

namespace detail
{

template<typename T>
class AtomicIntegerT : noncopyable {
	private:
		volatile std::atomic<T> value_;
	public:
		AtomicIntegerT()
			: value_(0)
		{
		}
		

		T get()
		{
			// in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
			return value_.load();
		}

		T getAndAdd(T x)
		{
			// in gcc >= 4.7: __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST)
			return value_.fetch_add(x);
			//return std::atomic_fetch_add_explicit(&value_, std::memory_order_seq_cst);
		}

		T addAndGet(T x)
		{
			return getAndAdd(x) + x;
		}

		T incrementAndGet()
		{
			return addAndGet(1);
		}

		T decrementAndGet()
		{
			return addAndGet(-1);
		}

		void add(T x)
		{
			getAndAdd(x);
		}

		void increment()
		{
			incrementAndGet();
		}

		void decrement()
		{
			decrementAndGet();
		}

		T getAndSet(T newValue)
		{
			// in gcc >= 4.7: __atomic_exchange_n(&value_, newValue, __ATOMIC_SEQ_CST)
			return  value_.exchange(newValue);
		}
};
} // namespace detail
typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
} // namespace muduo
