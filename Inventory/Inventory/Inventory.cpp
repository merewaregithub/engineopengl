#include <iostream>
#include "Item.h"
#include <string>
#include <vector>
#include <algorithm>
class Inventory
{
	std::vector<Item> inventoryitems;
	bool open;
public:
	void pick(Item i, int count);
	void drop(Item i, int count);
	void printCount(Item i);
	
};

void Inventory::pick(Item i, int count)
{
	for (int m = 0; m < count; m++)
	{
		inventoryitems.push_back(i);
	}

}

void Inventory::drop(Item i, int count)
{
	for (int m = 0; m < count; m++)
	{
		inventoryitems.erase(std::remove(inventoryitems.begin(), inventoryitems.end(), i), inventoryitems.end());
	}
}

void Inventory::printCount(Item i)
{
	std::cout << "Number of items: " << std::count(inventoryitems.begin(), inventoryitems.end(), i);
}

