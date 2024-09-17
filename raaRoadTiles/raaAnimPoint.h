#pragma once

#include <windows.h>
#include <osg/Vec3>
#include <osg/Quat>

class raaAnimPoint
{
public:
	raaAnimPoint();
	raaAnimPoint(osg::Vec3 vPos, osg::Quat qRot);
	virtual ~raaAnimPoint();

	osg::Vec3 pos();
	osg::Quat rot();

	raaAnimPoint& operator=(raaAnimPoint& ap);

	
protected:
	osg::Vec3 m_vPos;
	osg::Quat m_qRot;
};

