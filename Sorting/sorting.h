#ifndef SORTING_H
#define SORTING_H

#include <functional>
#include <mutex>

namespace sorting
{
	constexpr auto defaultComp = std::less();

	template <typename ForwardIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
	void bubbleSort(ForwardIt begin, ForwardIt end, Compare comp = defaultComp, std::function<void()> callback = nullptr)
	{
		ForwardIt currentEnd = end - 1;
		ForwardIt newEnd;

		do
		{
			newEnd = begin;
			for (ForwardIt it = begin; it != currentEnd; ++it)
			{
				if (comp(*(it + 1), *it))
				{
					std::iter_swap(it, it + 1);
					newEnd = it;

					if constexpr (callbackPresent)
					{
						callback();
					}
				}
			}
			currentEnd = newEnd;
		} while (currentEnd > begin);
	}

	template <typename RandomIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
	void quickSortInternal(RandomIt begin, RandomIt end, Compare comp = defaultComp, std::function<void()> callback = nullptr)
	{
		if (begin < end)
		{
			RandomIt pivotIt = begin;
			std::advance(pivotIt, std::distance(begin, end) / 2);
			const auto pivot = *pivotIt;
			RandomIt asc = begin, desc = end;
			RandomIt partition;

			for (;;)
			{
				while (comp(*asc, pivot)) 
				{ 
					++asc; 
				}
				while (comp(pivot, *desc)) 
				{ 
					--desc; 
				}
				if (asc >= desc)
				{
					partition = desc;
					break;
				}
				std::iter_swap(asc, desc);

				if constexpr (callbackPresent)
				{
					callback();
				}
			
				++asc;
				--desc;
			}

			quickSortInternal<RandomIt, Compare, callbackPresent>(begin, partition, comp, callback);
			quickSortInternal<RandomIt, Compare, callbackPresent>(partition + 1, end, comp, callback);
		}
	}

	template <typename RandomIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
		void quickSort(RandomIt begin, RandomIt end, Compare comp = defaultComp, std::function<void()> callback = nullptr)
	{
		quickSortInternal<RandomIt, Compare, callbackPresent>(begin, end - 1, comp, callback);
	}
}

#endif // SORTING_H