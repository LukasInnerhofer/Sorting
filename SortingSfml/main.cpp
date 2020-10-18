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
void drawBarGraph(sf::RenderTarget& target, RandomIt begin, RandomIt end, std::vector<std::vector<RandomIt>>& highlight)
{
	static sf::RectangleShape rect;
	static const std::vector<sf::Color> highlightColors = { sf::Color::Magenta, sf::Color::Cyan, sf::Color::Yellow };

	const size_t dataSize = end - begin;
	const decltype(*(std::declval<RandomIt>())) max = *std::max_element(begin, end);

	for (RandomIt it = begin; it != end; ++it)
	{
		rect.setFillColor(sf::Color::White);
		for (size_t itHighlightClass = 0; itHighlightClass < highlight.size(); ++itHighlightClass)
		{
			for (size_t itHighlight = 0; itHighlight < highlight[0].size(); ++itHighlight)
			{
				if (it == highlight[itHighlightClass][itHighlight])
				{
					rect.setFillColor(highlightColors[itHighlightClass]);
				}
			}
		}

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

template <typename Collection, typename Mutex>
void render(Collection& collection, std::vector<std::vector<decltype(std::declval<Collection>().begin())>>& highlight, Mutex& mutex, std::condition_variable& condition)
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

			drawBarGraph(window, collection.begin(), collection.end(), highlight);
			mutex.unlock();
			condition.notify_one();

			window.display();
		}

		timerThread.join();
	}
}

template <typename ForwardIt, typename BaseType>
void randomize(ForwardIt begin, ForwardIt end, BaseType lower, BaseType upper)
{
	std::random_device rd;
	std::default_random_engine randomEngine(rd());
	std::uniform_int_distribution<int> dist(lower, upper);

	for (ForwardIt it = begin; it != end; ++it)
	{
		*it = dist(randomEngine);
	}
}

int main()
{
	using BaseType = int;
	std::vector<BaseType> collection(windowWidth / 2);
	using RandomIt = decltype(collection.begin());
	std::vector<std::vector<RandomIt>> highlight;
	std::mutex mutex;
	std::condition_variable condition;

	randomize(collection.begin(), collection.end(), 0U, windowHeight);

	auto renderThread = std::thread(
		render<decltype(collection), std::mutex>,
		std::ref(collection),
		std::ref(highlight),
		std::ref(mutex),
		std::ref(condition)
	);

	auto sortThread = std::thread(
		[&]() 
		{
			std::unique_lock<std::mutex> lock(mutex);
			sorting::quickSort<RandomIt, decltype(sorting::defaultComp), true>(
				collection.begin(),
				collection.end(),
				sorting::defaultComp,
				[&](const std::vector<std::vector<RandomIt>>& cbHighlight)
				{
					static unsigned int counter = 0;
					if (++counter >= 1)
					{
						counter = 0;
						highlight = cbHighlight;
						condition.wait(lock);
					}
				});
		}
	);

	sortThread.join();

	std::this_thread::sleep_for(std::chrono::seconds(2));

	{
		std::lock_guard<std::mutex> lock(mutex);
		highlight.clear();
		randomize(collection.begin(), collection.end(), 0U, windowHeight);
	}

	sortThread = std::thread(
		[&]()
		{
			std::unique_lock<std::mutex> lock(mutex);
			std::sort(collection.begin(), collection.end(),
				[&](BaseType lhs, BaseType rhs)
				{
					static unsigned int counter = 0;
					if (++counter >= 4)
					{
						counter = 0;
						condition.wait(lock);
					}
					return lhs < rhs;
				});
		}
	);

	sortThread.join();

	std::this_thread::sleep_for(std::chrono::seconds(2));

	{
		std::lock_guard<std::mutex> lock(mutex);
		highlight.clear();
		collection.resize(windowWidth / 10);
		randomize(collection.begin(), collection.end(), 0U, windowHeight);
	}

	sortThread = std::thread(
		[&]()
		{
			std::unique_lock<std::mutex> lock(mutex);
			sorting::bubbleSort<RandomIt, decltype(sorting::defaultComp), true>(
				collection.begin(),
				collection.end(),
				sorting::defaultComp,
				[&](const std::vector<std::vector<RandomIt>>& cbHighlight)
				{
					static unsigned int counter = 0;
					if (++counter >= 1)
					{
						counter = 0;
						highlight = cbHighlight;
						condition.wait(lock);
					}
				});
		}
	);

	sortThread.join();
	renderThread.join();

	return 0;
}