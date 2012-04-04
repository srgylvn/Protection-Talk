#include <map>

namespace utils
{
	template <class TFirst, class TSecond>
	TSecond MapGetOrValue(std::map<TFirst, TSecond> & mapData, const TFirst & key, const TSecond & value)
	{
		std::map<TFirst, TSecond>::iterator iter = mapData.find(key);
		if (iter == mapData.end())
			return value;

		return iter->second;
	}
}