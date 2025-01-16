#pragma once
#include <string>
using namespace std;

class Coffee
{
protected:
	string name;
	double price;
	int prepTime; // 制作时间(分钟)

public:
	Coffee(string n, double p, int t) : name(n), price(p), prepTime(t) {}
	virtual ~Coffee() {}

	string getName() const { return name; }
	double getPrice() const { return price; }
	int getPrepTime() const { return prepTime; }
};

class Americano : public Coffee
{
public:
	Americano() : Coffee("Americano", 25.0, 3) {}
};

class Latte : public Coffee
{
public:
	Latte() : Coffee("Latte", 30.0, 4) {}
};

class Little_Iron : public Coffee
{
public:
	Little_Iron() : Coffee("Little_Iron", 35.0, 5) {}
};

class Tea_Coffee : public Coffee
{
public:
	Tea_Coffee() : Coffee("Tea_Coffee", 28.0, 2) {}
};

class Fruit_Coffee : public Coffee
{
public:
	Fruit_Coffee() : Coffee("Fruit_Coffee", 32.0, 2) {}
};

class Cappuccino : public Coffee
{
public:
	Cappuccino() : Coffee("Cappuccino", 33.0, 3) {}
};
