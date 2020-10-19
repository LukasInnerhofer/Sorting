#ifndef SORTING_H
#define SORTING_H

#include <functional>
#include <mutex>

namespace sorting
{
	constexpr auto defaultComp = std::less();

	template <typename ForwardIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
	void bubbleSort(ForwardIt begin, ForwardIt end, Compare comp = defaultComp, std::function<void(const std::vector<std::vector<ForwardIt>>&)> callback = nullptr)
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
						static auto highlight = std::vector<std::vector<ForwardIt>>({ std::vector<ForwardIt>(1) });
						highlight[0][0] = it + 1;
						callback(highlight);
					}
				}
			}
			currentEnd = newEnd;
		} while (currentEnd > begin);
	}

	template <typename RandomIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
	void quickSortInternal(RandomIt begin, RandomIt end, Compare comp = defaultComp, std::function<void(const std::vector<std::vector<RandomIt>>&)> callback = nullptr)
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
					if (asc == pivotIt)
					{
						pivotIt = desc;
					}
					else if (desc == pivotIt)
					{
						pivotIt = asc;
					}

					static auto highlight = std::vector<std::vector<RandomIt>>(3, std::vector<RandomIt>(1));
					highlight[0][0] = pivotIt;
					highlight[1][0] = asc;
					highlight[2][0] = desc;
					callback(highlight);
				}
			
				++asc;
				--desc;
			}

			quickSortInternal<RandomIt, Compare, callbackPresent>(begin, partition, comp, callback);
			quickSortInternal<RandomIt, Compare, callbackPresent>(partition + 1, end, comp, callback);
		}
	}

	template <typename RandomIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
	void quickSort(RandomIt begin, RandomIt end, Compare comp = defaultComp, std::function<void(const std::vector<std::vector<RandomIt>>&)> callback = nullptr)
	{
		quickSortInternal<RandomIt, Compare, callbackPresent>(begin, end - 1, comp, callback);
	}

	template <typename ForwardIt, typename Compare = decltype(defaultComp), bool callbackPresent = false>
	void selectionSort(ForwardIt begin, ForwardIt end, Compare comp = defaultComp, std::function<void(const std::vector<std::vector<ForwardIt>>&)> callback = nullptr)
	{
		ForwardIt itMin;

		for (ForwardIt itOuter = begin; itOuter != end; ++itOuter)
		{
			itMin = itOuter;

			for (ForwardIt itInner = itOuter; itInner != end; ++itInner)
			{
				if (comp(*itInner, *itMin))
				{
					itMin = itInner;
				}
			}

			std::iter_swap(itOuter, itMin);

			if constexpr (callbackPresent)
			{
				static std::vector<std::vector<ForwardIt>> highlight(1, std::vector<ForwardIt>(2));
				highlight[0][0] = itOuter;
				highlight[0][1] = itMin;
				callback(highlight);
			}
		}
	}
}

#endif // SORTING_H