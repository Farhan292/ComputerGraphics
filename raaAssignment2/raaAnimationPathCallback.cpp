#include <windows.h>
#include <osg/NodeVisitor>

#include "raaAnimationPathCallback.h"

raaAnimationPathCallback::raaAnimationPathCallback(osg::AnimationPath* ap, double dSpeed) : osg::AnimationPathCallback(ap, 0.0, 1.0), m_dSpeed(dSpeed), m_bFirst(true), m_dTimeStep(0.0), m_dActualTime(0.0)
{
}

raaAnimationPathCallback::~raaAnimationPathCallback()
{
}

double raaAnimationPathCallback::getAnimationTime() const
{
	return m_dActualTime;
}

void raaAnimationPathCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if(m_bFirst)
	{
		m_dLastTime = nv->getFrameStamp()->getSimulationTime();
		m_bFirst = false;
	}
	else
	{
		double dTime = nv->getFrameStamp()->getSimulationTime();
		m_dTimeStep = (dTime - m_dLastTime) * m_dSpeed;
		if (!_pause)
		{
			m_dActualTime += m_dTimeStep;
			update(*node);
		}
		m_dLastTime = dTime;
	}
	NodeCallback::traverse(node, nv);

}

double raaAnimationPathCallback::getCurrentSpeed()
{
	return m_dSpeed;
}

void raaAnimationPathCallback::setCurrentSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
}

double raaAnimationPathCallback::getLastTimeStep()
{
	return m_dTimeStep;
}
