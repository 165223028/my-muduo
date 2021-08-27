#pragma once
#include "Types.h"

namespace muduo{
class Timestamp
{
	private:
		int64_t microSecondsSinceEpoch_; //ms
	public:
		Timestamp(): microSecondsSinceEpoch_(0){}

		explicit Timestamp(int64_t microSecondsSinceEpochArg)
			: microSecondsSinceEpoch_(microSecondsSinceEpochArg){}

		//tool
		static const int kMicroSecondsPerSecond = 1000 * 1000;

		int64_t microSecondsSinceEpoch() const 
		{ 
			return microSecondsSinceEpoch_; 
		}

		time_t secondsSinceEpoch() const
		{
			return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
		}

		//get time
		static Timestamp now();
		static Timestamp invalid() //无效时间
		{
			return Timestamp();
		}

		static Timestamp fromUnixTime(time_t t)
		{
			return fromUnixTime(t, 0);
		}

		static Timestamp fromUnixTime(time_t t, int microseconds)
		{
			return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
		}

		void swap(Timestamp& rhs)
		{
			std::swap(microSecondsSinceEpoch_, rhs.microSecondsSinceEpoch_);
		}
		
		string toString() const;
		string toFormattedString(bool showMicroseconds = true) const;
		
		bool valid() const { return microSecondsSinceEpoch_ > 0; }
};

inline bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator<=(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator==(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}
inline bool operator>(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.microSecondsSinceEpoch() > rhs.microSecondsSinceEpoch();
}
inline bool operator>=(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.microSecondsSinceEpoch() >= rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
	int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}
}
