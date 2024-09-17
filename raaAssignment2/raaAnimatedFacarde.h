#pragma once

#include "raaAnimationPathCallback.h"
#include "raaFacarde.h"

class raaAnimatedFacarde: public raaFacarde, public raaAnimationPathCallback
{
public:
	raaAnimatedFacarde(osg::Node* pPart, osg::AnimationPath* ap, double dMaxSpeed);
	virtual ~raaAnimatedFacarde();
};

