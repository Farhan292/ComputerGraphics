#pragma once
#include "raaNodeCallBackFacarde.h"
#include "TrafficLightFacarde.h"
#include <list>

typedef std::list<TrafficLightFacarde*> trafficLightList;

class TrafficLightControl : public raaNodeCallbackFacarde
{
public:
	TrafficLightControl(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale);
	virtual ~TrafficLightControl();
	void operator() (osg::Node* node, osg::NodeVisitor* nv) override;
	void addTrafficLight(TrafficLightFacarde* pTrafficLight);
	void changeTrafficLight(TrafficLightFacarde* pTrafficLight);

protected:
	trafficLightList m_lTrafficLights;
	int timeCount;
};
