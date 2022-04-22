#include "union-manager.h"

UnionManager::UnionManager(std::vector<AbstractManager*> managers) : managers(managers) {}

bool UnionManager::poll()
{
	for (std::size_t i = 0; i < managers.size(); ++i)
		managers[i]->poll();
	return true;
}
