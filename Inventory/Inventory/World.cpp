#include <iostream>
#include <vector>
#include "Item.h"
#include <string>
#include <algorithm>
class World
{
	std::vector<Item> worlditems;

public:
	
	void pick(Item i, int count);
	void drop(Item i, int count);
	void put(Item i, int count);
	std::string printAll();
};


void World::pick(Item i, int count) 
{
	for (int m = 0; m < count; m++)
	{
		worlditems.push_back(i);
	}
}

void World::drop(Item i, int count)
{
	for (int m = 0; m < count; m++)
	{
		worlditems.erase(std::remove(worlditems.begin(), worlditems.end(), i), worlditems.end());
	}
}

void World::put(Item i, int count)
{

}
