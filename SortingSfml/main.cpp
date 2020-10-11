#include "sorting.h"

#include <iostream>
#include <vector>

int main()
{
	std::vector<int> v{ 10, 1, 4 };

	sorting::bubbleSort(v.begin(), v.end(),
		std::function<void(decltype(v.begin()), decltype(v.end()))>(
			[](std::vector<int>::iterator begin, std::vector<int>::iterator end) -> void
			{
				for (std::vector<int>::iterator it = begin; it != end; ++it)
				{
					std::cout << *it << "\n";
				}
			}));

	return 0;
}