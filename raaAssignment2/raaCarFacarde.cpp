
#include <windows.h>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Material>
#include <osg/Switch>

#include "raaTrafficSystem.h"
#include "raaCarFacarde.h"
#include "TrafficLightFacarde.h"
#include <iostream>
#include "raaFacarde.h"

raaCarFacarde::raaCarFacarde(osg::Node* pWorldRoot, osg::Node* pPart, osg::AnimationPath* ap, double dSpeed) : raaAnimatedFacarde(pPart, ap, dSpeed)
{
	raaTrafficSystem::addTarget(this); // adds the car to the traffic system (static class) which holds a reord of all the dynamic parts in the system
}

raaCarFacarde::~raaCarFacarde()
{
	raaTrafficSystem::removeTarget(this); // removes the car from the traffic system (static class) which holds a reord of all the dynamic parts in the system
}

void raaCarFacarde::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    const raaFacardes& facardes = raaFacarde::getFacardes();

    //Minimum distance for collison
    float carMin = 200.0f;
    float trafficLightMin = 300.0f;

    for (auto facarde : facardes)
    {
        //CHECK if facarde is car facarde
        raaCarFacarde* carFacarde = dynamic_cast <raaCarFacarde*>(facarde);
        if (carFacarde)
        {
            float carSpeed = carFacarde->getSpeed();
            osg::Vec3f colliderPosition = carFacarde->getWorldDetectionPoint();
            osg::Vec3f thisPosition = this->getWorldDetectionPoint();

            float dx = colliderPosition.x() - thisPosition.x();
            float dy = colliderPosition.y() - thisPosition.y();
            float dz = colliderPosition.z() - thisPosition.z();

            //Distance
            float distance = sqrt(dx * dx + dy * dy + dz * dz);

            if (distance < carMin)
            {
                //If its within the minimum distance - match speed of other car
                this->m_dSpeed = carSpeed;
            }
            
        }
        else
        {
            //CHECK facarde is traffic light
            TrafficLightFacarde* trafficLightFacarde = dynamic_cast <TrafficLightFacarde*>(facarde);
            if (trafficLightFacarde)
            {
                osg::Vec3f colliderPosition = trafficLightFacarde->getWorldDetectionPoint();
                osg::Vec3f thisPosition = this->getWorldDetectionPoint();
                float dx = colliderPosition.x() - thisPosition.x();
                float dy = colliderPosition.y() - thisPosition.y();
                float dz = colliderPosition.z() - thisPosition.z();
                float distance = sqrt(dx * dx + dy * dy + dz * dz);
                if (distance < trafficLightMin)
                {
                    if (trafficLightFacarde->getCurrentTrafficLightStatus() == 1)
                    {
                        //Stops at red light
                        this->m_dSpeed = 0.0f;
                    }
                    else
                    {
                        //Slows down near traffic light
                        this->m_dSpeed = this->m_dSpeed-5.0f;
                    }
                }
            }
        }
    }
    // Call the parent operator() to handle animation path callback
    raaAnimationPathCallback::operator()(node, nv);
}
osg::Vec3f raaCarFacarde::getWorldDetectionPoint()
{
    //return osg::Vec3(); // should return the world position of the detection point for this subtree
    return this->m_pRoot->getBound().center(); // should return the world position of the detection point for this subtree
}

osg::Vec3f raaCarFacarde::getWorldCollisionPoint()
{
    return osg::Vec3(); // should return the world position of the collision point for this subtree
}
float raaCarFacarde::getSpeed() const
{
    return m_fSpeed;
}

