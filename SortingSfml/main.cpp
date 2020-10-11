#include "sorting.h"
#include "SFML/Graphics.hpp"

#include <iostream>
#include <vector>
#include <thread>

template <typename RandomIt>
void drawBarGraph(sf::RenderTarget& target, RandomIt begin, RandomIt end)
{
	static sf::RectangleShape rect;

	const size_t dataSize = end - begin;
	const decltype(*(std::declval<RandomIt>())) max = *std::max_element(begin, end);

	rect.setFillColor(sf::Color::White);

	for (RandomIt it = begin; it != end; ++it)
	{
		rect.setSize(
			{ 
				static_cast<float>(target.getSize().x) / dataSize, 
				(static_cast<float>(*it) / max) * target.getSize().y
			});
		rect.setPosition(
			{
				(static_cast<float>(it - begin) / dataSize) * target.getSize().x,
				static_cast<float>(target.getSize().y - rect.getSize().y)
			});
		target.draw(rect);
	}
}

int main()
{
	std::vector<int> v{ 111, 10, 1, 4 };

	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "Sorting");
	sf::Event e;
	bool redraw = true;

	while (window.isOpen())
	{
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		if (redraw)
		{
			redraw = false;
			window.clear();
			
			drawBarGraph(window, v.cbegin(), v.cend());
			window.display();
		}
	}

	return 0;
}