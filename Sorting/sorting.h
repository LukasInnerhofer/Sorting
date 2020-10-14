#ifndef SORTING_H
#define SORTING_H

#include <functional>
#include <mutex>

constexpr auto bubbleSortDefaultComp = std::greater();

namespace sorting
{
	template <typename RandomIt, typename Compare, bool callbackPresent>
	void bubbleSort(RandomIt begin, RandomIt end, Compare comp, std::function<void(RandomIt begin, RandomIt end)> callback = nullptr)
	{
		RandomIt currentEnd = end - 1;
		RandomIt newEnd;

		do
		{
			newEnd = begin;
			for (RandomIt it = begin; it != currentEnd; ++it)
			{
				if (comp(*it, *(it + 1)))
				{
					std::iter_swap(it, it + 1);
					newEnd = it;

					if constexpr (callbackPresent)
					{
						callback(begin, end);
					}
				}
			}
			currentEnd = newEnd;
		} while (currentEnd > begin);
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

	template <typename RandomIt, typename Mutex, typename Compare>
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