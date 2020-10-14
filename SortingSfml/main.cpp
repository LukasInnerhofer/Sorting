#include "sorting.h"
#include "SFML/Graphics.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <vector>
#include <thread>

static constexpr unsigned int windowWidth = 800;
static constexpr unsigned int windowHeight = 600;

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

template <typename RandomIt, typename Mutex>
void render(RandomIt begin, RandomIt end, Mutex& mutex)
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(windowWidth, windowHeight), "Sorting");
	sf::Event e;

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

		if (mutex.try_lock())
		{
			window.clear();

			drawBarGraph(window, begin, end);
			mutex.unlock();

			window.display();
		}

		timerThread.join();
	}
}

int main()
{
	std::vector<int> collection(windowWidth);
	using RandomIt = decltype(collection.begin());
	std::mutex mutex;

	std::random_device rd;
	std::default_random_engine randomEngine(rd());
	std::uniform_int_distribution<int> dist(0, windowHeight);

	for (auto it = collection.begin(); it != collection.end(); ++it)
	{
		*it = dist(randomEngine);
	}

	void (*sort)(RandomIt, RandomIt, std::mutex&, std::function<void(RandomIt, RandomIt, std::mutex&)>) = sorting::bubbleSort;
	auto sortThread = std::thread(
		sort,
		collection.begin(),
		collection.end(),
		std::ref(mutex),
		[](RandomIt begin, RandomIt end, std::mutex& mutex)
		{
			static unsigned int counter = 0;

			if (++counter == 40)
			{
				counter = 0;
				mutex.unlock();
				std::this_thread::sleep_for(std::chrono::microseconds(500));
				mutex.lock();
			}
		}
	);

	auto renderThread = std::thread(
		render<RandomIt, std::mutex>,
		collection.begin(),
		collection.end(),
		std::ref(mutex)
	);

	sortThread.join();
	renderThread.join();

	return 0;
}