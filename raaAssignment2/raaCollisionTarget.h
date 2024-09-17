#pragma once

#include <windows.h>
#include <osg/Vec3f>

// a pure virtual base class (interface) to provide support for collidable objects

class raaCollisionTarget
{
public:
    virtual osg::Vec3f getWorldDetectionPoint() = 0;
    virtual osg::Vec3f getWorldCollisionPoint() = 0;
};
