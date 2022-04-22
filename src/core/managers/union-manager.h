#ifndef UNION_MANAGER_H_
#define UNION_MANAGER_H_


#include <vector>

#include "core/managers/abstract-manager.h"

class UnionManager : public AbstractManager
{
private:
	std::vector<AbstractManager*> managers;

public:
	UnionManager(std::vector<AbstractManager*> managers);
	virtual ~UnionManager() = default;

public:
	bool poll() override;
};


#endif /* UNION_MANAGER_H_ */
