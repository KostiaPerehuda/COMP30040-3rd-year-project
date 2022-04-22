#include "application-container.h"

ApplicatonContainer::ApplicatonContainer() : application(nullptr) {}

ApplicatonContainer::ApplicatonContainer(AbstractManager* application) : application(application) {}

void ApplicatonContainer::setApplicaton(AbstractManager* applicationIn)
{
	application = applicationIn;
}

AbstractManager* ApplicatonContainer::getApplicaton()
{
	return application;
}
