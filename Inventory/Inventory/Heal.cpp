#include "Item.h"


class Heal : public Item
{
	std::string type;
	int max_stack_count = std::numeric_limits<int>::infinity();
};
