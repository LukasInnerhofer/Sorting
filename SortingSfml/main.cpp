#include "sorting.h"
#include "SFML/Graphics.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
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
	std::vector<int> v(800);

	std::random_device rd;
	std::default_random_engine randomEngine(rd());
	std::uniform_int_distribution<int> dist(0, 600);

	for (auto it = v.begin(); it != v.end(); ++it)
	{
		*it = dist(randomEngine);
	}

	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "Sorting");
	sf::Event e;

	std::atomic<bool> mainThreadLock = false, sortThreadLock = false;;

	auto callback = [&](std::vector<int>::iterator, std::vector<int>::iterator) 
	{ 
		sortThreadLock = true;
		while (mainThreadLock) {};
		sortThreadLock = false;
	};

	auto sortThread = std::thread(
		sorting::bubbleSort<std::vector<int>::iterator>,
		v.begin(),
		v.end(),
		callback
	);
	std::thread timerThread;

	while (window.isOpen())
	{
		timerThread = std::thread([]() { std::this_thread::sleep_for(std::chrono::milliseconds(16)); });

		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		
		if (!sortThreadLock)
		{
			mainThreadLock = true;
			window.clear();

			drawBarGraph(window, v.cbegin(), v.cend());
			mainThreadLock = false;

			window.display();
		}
		

		timerThread.join();
	}

	sortThread.join();

	return 0;
}