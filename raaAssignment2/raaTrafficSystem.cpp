#include "raaCollisionTarget.h"
#include "raaTrafficSystem.h"

raaCollisionTargets raaTrafficSystem::sm_lColliders;

void raaTrafficSystem::start()
{
	sm_lColliders.clear();
}

void raaTrafficSystem::end()
{
	sm_lColliders.clear();
}

void raaTrafficSystem::addTarget(raaCollisionTarget* pTarget)
{
	if (pTarget && std::find(sm_lColliders.begin(), sm_lColliders.end(), pTarget) == sm_lColliders.end()) sm_lColliders.push_back(pTarget);
}

void raaTrafficSystem::removeTarget(raaCollisionTarget* pTarget)
{
	if (pTarget && std::find(sm_lColliders.begin(), sm_lColliders.end(), pTarget) != sm_lColliders.end()) sm_lColliders.remove(pTarget);
}

const raaCollisionTargets& raaTrafficSystem::colliders()
{
	return sm_lColliders;
}