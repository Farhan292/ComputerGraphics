#pragma once

#include <windows.h>
#include <osg/AnimationPath>

// this is a derived version of the osg::AnimationPathCallback. Yoou may wish to use this as it gives control over the speed of the animated playback

class raaAnimationPathCallback: public osg::AnimationPathCallback
{
public:
	raaAnimationPathCallback(osg::AnimationPath* ap, double dSpeed);
	virtual ~raaAnimationPathCallback();
	double getAnimationTime() const override;
	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;

	double getCurrentSpeed();
	void setCurrentSpeed(double dSpeed);
	double getLastTimeStep();
	
protected:
	double m_dSpeed;
private:
	double m_dLastTime;
	double m_dTimeStep;
	double m_dActualTime;
	bool m_bFirst;
};

