#pragma once

#include <list>

class raaCollisionTarget;

typedef std::list<class raaCollisionTarget*> raaCollisionTargets;

class raaTrafficSystem
{
public:
    static void start();
    static void end();
    static void addTarget(raaCollisionTarget* pTarget);
    static void removeTarget(raaCollisionTarget* pTarget);
    static const raaCollisionTargets& colliders();

protected:
    static raaCollisionTargets sm_lColliders;
};
