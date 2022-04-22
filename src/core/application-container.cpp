#include "application-container.h"

void ApplicatonContainer::setApplicaton(AbstractManager* applicationIn)
{
	application = applicationIn;
}

AbstractManager* ApplicatonContainer::getApplicaton()
{
	return application;
}
