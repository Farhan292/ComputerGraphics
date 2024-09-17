#include "raaAnimPoint.h"

raaAnimPoint::raaAnimPoint()
{
}

raaAnimPoint::raaAnimPoint(osg::Vec3 vPos, osg::Quat qRot): m_vPos(vPos), m_qRot(qRot)
{
	
}

raaAnimPoint::~raaAnimPoint()
{
}

osg::Vec3 raaAnimPoint::pos()
{
	return m_vPos;
}

osg::Quat raaAnimPoint::rot()
{
	return m_qRot;
}

raaAnimPoint& raaAnimPoint::operator=(raaAnimPoint& ap)
{
	m_vPos = ap.pos();
	m_qRot = ap.rot();

	return ap;
}
