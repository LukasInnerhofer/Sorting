#ifndef SORTING_H
#define SORTING_H

#include <functional>
#include <mutex>

constexpr auto bubbleSortDefaultComp = std::less_equal();

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
	void bubbleSort(RandomIt begin, RandomIt end, Compare comp = bubbleSortDefaultComp, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
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

	template <typename RandomIt, typename Compare, typename Mutex>
	void bubbleSort(RandomIt begin, RandomIt end, Mutex& mutex, Compare comp = bubbleSortDefaultComp, std::function<void(RandomIt, RandomIt, Mutex&)> callback = nullptr)
	{
		mutex.lock();
		bubbleSort(
			begin, 
			end, 
			comp, 
			std::function<void(RandomIt, RandomIt)>([&](RandomIt begin, RandomIt end) { if(callback) callback(begin, end, mutex); }
		));
		mutex.unlock();
	}

	template <typename RandomIt, typename Mutex>
	void bubbleSort(RandomIt begin, RandomIt end, Mutex& mutex, std::function<void(RandomIt, RandomIt, Mutex&)> callback = nullptr)
	{
		bubbleSort(begin, end, mutex, bubbleSortDefaultComp, callback);
	}
}

#endif // SORTING_H