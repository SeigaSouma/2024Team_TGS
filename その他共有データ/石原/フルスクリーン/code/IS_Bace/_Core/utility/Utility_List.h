//======================================================
//
//ï÷óòÇ»ÉäÉXÉg[Utility_List.h]
//Author:êŒå¥ÈDîn
//
//======================================================
#ifndef _UTILITY_LIST_H_
#define _UTILITY_LIST_H_
#include <list>
#include <algorithm>
#include <functional>

namespace IS_Utility
{
	template<typename T> class Utility_List : public std::list<T>
	{
	public:
		
		Utility_List<T> Filtered(std::function<T(T)> func)
		{
			Utility_List<T> removed = *this;
			auto it = std::remove_if(removed.begin(), removed.end(), func);
			removed.erase(it, removed.end());
			return removed;
		}
	};
}

#endif // _UTILITY_LIST_H_
