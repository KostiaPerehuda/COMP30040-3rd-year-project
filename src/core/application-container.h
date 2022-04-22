#ifndef APPLICATION_CONTAINER_H_
#define APPLICATION_CONTAINER_H_


#include "core/managers/abstract-manager.h"

class ApplicatonContainer
{
private:
	AbstractManager* application;

public:
	virtual ~ApplicatonContainer() = default;

public:
	void setApplicaton(AbstractManager* application);
	AbstractManager* getApplicaton();
};


#endif /* APPLICATION_CONTAINER_H_ */
