#ifndef SORTING_H
#define SORTING_H

#include <functional>

namespace sorting
{
	template <typename RandomIt, bool callbackPresent>
	void bubbleSort(RandomIt begin, RandomIt end, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
	{
		bool done = false;

		do
		{
			for (RandomIt it = begin; it != end - 1; ++it)
			{
				if()
			}
		} while(!done)

		if constexpr (callbackPresent)
		{
			if (callback)
			{
				callback(begin, end);
			}
		}
	}

	template <typename RandomIt>
	void bubbleSort(RandomIt begin, RandomIt end, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
	{
		if (callback)
		{
			bubbleSort<RandomIt, true>(begin, end, callback);
		}
		else
		{
			bubbleSort<RandomIt, false>(begin, end);
		}
	}
}

#endif // SORTING_H