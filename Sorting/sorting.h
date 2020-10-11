#ifndef SORTING_H
#define SORTING_H

#include <functional>

namespace sorting
{
	template <typename RandomIt, typename Compare, bool callbackPresent>
	void bubbleSort(RandomIt begin, RandomIt end, Compare comp, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
	{
		bool done;

		do
		{
			done = true;
			for (RandomIt it = begin; it != end - 1; ++it)
			{
				if (!comp(*it, *(it + 1)))
				{
					std::iter_swap(it, it + 1);
					done = false;

					if constexpr (callbackPresent)
					{
						callback(begin, end);
					}
				}
			}
		} while (!done);
	}

	template <typename RandomIt, typename Compare>
	void bubbleSort(RandomIt begin, RandomIt end, Compare comp, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
	{
		if (callback)
		{
			bubbleSort<RandomIt, Compare, true>(begin, end, comp, callback);
		}
		else
		{
			bubbleSort<RandomIt, Compare, false>(begin, end, comp);
		}
	}

	template <typename RandomIt>
	void bubbleSort(RandomIt begin, RandomIt end, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
	{
		bubbleSort(begin, end, std::less<>(), callback);
	}
}

#endif // SORTING_H