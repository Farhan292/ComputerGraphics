#pragma once

#include <windows.h>
#include <osg/NodeVisitor>
#include <string>

class raaAnimationPointFinder
{
public:
	raaAnimationPointFinder(std::string sTileName, unsigned int uiAnimPoint, osg::Node* pRoot);
	virtual ~raaAnimationPointFinder();

	osg::MatrixTransform* animationPoint();;

	const raaAnimationPointFinder& operator=(const raaAnimationPointFinder& ap);

	osg::Vec3 translation();
	osg::Quat rotation();
	int nodePathFinder();
	
protected:
	std::string m_sTile;
	unsigned int m_uiAnimPoint;
	osg::Node* m_pRoot;
};

