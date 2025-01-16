#pragma once
#include <vector>
#include <string>
#include <ctime>
#include "Coffee.h"
#include <algorithm> // 使用 min 和 max 函数
using namespace std;

class Order
{
private:
	vector<pair<Coffee *, int>> items; // 咖啡及其数量
	int totalTime;
	double totalPrice;
	bool isProcessing;
	time_t startTime;
	int totalPrepTime;

public:
	Order() : totalTime(0), totalPrice(0), isProcessing(false), startTime(0), totalPrepTime(0) {}

	void addItem(Coffee *coffee, int quantity)
	{
		items.push_back({coffee, quantity});
		totalTime += coffee->getPrepTime() * quantity;
		totalPrice += coffee->getPrice() * quantity;
		totalPrepTime += coffee->getPrepTime() * quantity;
	}

	string getStatus() const
	{
		if (!isProcessing)
			return "Waiting";
		int remainingTime = totalTime - static_cast<int>(time(nullptr) - startTime);
		if (remainingTime > 0)
			return "Processing: " + to_string(remainingTime) + "s";
		return "Completed";
	}

	void startProcessing()
	{
		isProcessing = true;
		startTime = time(nullptr);
	}

	bool isCompleted() const
	{
		if (!isProcessing)
			return false;

		time_t currentTime = time(nullptr);
		double elapsedMinutes = difftime(currentTime, startTime) / 60.0;

		return elapsedMinutes >= totalPrepTime;
	}

	int getProgress() const
	{
		if (!isProcessing)
			return 0;
		if (isCompleted())
			return 100;

		time_t currentTime = time(nullptr);
		double elapsedMinutes = difftime(currentTime, startTime) / 60.0;

		int progress = static_cast<int>((elapsedMinutes / totalPrepTime) * 100);
		return min(100, max(0, progress));
	}

	double getTotalPrice() const { return totalPrice; }
	int getTotalTime() const { return totalTime; }
	time_t getStartTime() const { return startTime; }

	int getTotalItems() const
	{
		int total = 0;
		for (const auto &item : items)
		{
			total += item.second;
		}
		return total;
	}
};
