#include "TrafficLightControl.h"

TrafficLightControl::TrafficLightControl(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale) : raaNodeCallbackFacarde(pPart, vTrans, fRot, fScale)
{
	timeCount = 0;
}

TrafficLightControl::~TrafficLightControl()
{

}

void TrafficLightControl::operator() (osg::Node* node, osg::NodeVisitor* nv)
{
	if (timeCount == 100)
	{
		int noOfLightsUpdate = m_lTrafficLights.size();
		for (trafficLightList::iterator it = m_lTrafficLights.begin(); it != m_lTrafficLights.end(); it++)
		{
			changeTrafficLight(*it);
			if ((*it)->m_iTrafficLightStatus == 1)
			{
				noOfLightsUpdate--;
			}
		}

		timeCount = 0;
	}
	timeCount++;
}

void TrafficLightControl::changeTrafficLight(TrafficLightFacarde* pTrafficLight)
{
	pTrafficLight->m_iTrafficLightStatus++;
	if (pTrafficLight->m_iTrafficLightStatus > 3)
	{
		pTrafficLight->m_iTrafficLightStatus = 1;
	}
	if (pTrafficLight->m_iTrafficLightStatus == 1)
	{
		pTrafficLight->setRedTrafficLight();
	}
	if (pTrafficLight->m_iTrafficLightStatus == 2)
	{
		pTrafficLight->setAmberTrafficLight();
	}
	if (pTrafficLight->m_iTrafficLightStatus == 3)
	{
		pTrafficLight->setGreenTrafficLight();
	}
}

void TrafficLightControl::addTrafficLight(TrafficLightFacarde* pTrafficLight)
{
	m_lTrafficLights.push_back(pTrafficLight);
}
