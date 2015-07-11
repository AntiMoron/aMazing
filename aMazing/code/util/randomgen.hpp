#pragma once
#include"../common/CommonDef.hpp"
#include"../engine/system/thread/aThreadSafeSingleton.hpp"
#include<random>

namespace aMazing
{
	class RandomNumberGenerator
		: public aThreadSafeSingleton<RandomNumberGenerator>
	{
	private:
		std::random_device rd;
		std::mt19937_64 regen;
		std::uniform_int_distribution<long long> dist;
		RandomNumberGenerator(){}
		friend class aThreadSafeSingleton<RandomNumberGenerator>;
	public:
		int getInteger32() aNOEXCEPT
		{
			return dist(rd);
		}
		int getInteger32(int low,int high) aNOEXCEPT
		{
			return low + (getInteger32() % (high - low));
		}
		long long getInteger64() aNOEXCEPT
		{
			return dist(rd);
		}
		
		long long getInteger64(long long low, long long high) aNOEXCEPT
		{
			return low + (getInteger64() % (high - low));
		}
#undef min
#undef max
		float getFloat32() aNOEXCEPT
		{
			return float(dist(rd) / double(dist.max()));
		}

		double getFloat64() aNOEXCEPT
		{
			return dist(rd) / double(dist.max());
		}
	};
}

#define Random (aMazing::RandomNumberGenerator::getInstance())
