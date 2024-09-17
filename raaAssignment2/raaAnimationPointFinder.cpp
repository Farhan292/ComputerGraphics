#include <windows.h>
#include <sstream>
#include <iostream>

#include <osg/MatrixTransform>
#include "raaAnimationPointFinder.h"
#include "raaFinder.h"

raaAnimationPointFinder::raaAnimationPointFinder(std::string sTileName, unsigned uiAnimPoint, osg::Node* pRoot) : m_sTile(sTileName), m_uiAnimPoint(uiAnimPoint), m_pRoot(pRoot)
{
	// You should use a set of finders to find the specific anaimation point for a particular tile. 
	// Remember the animation points are instanced (common to shared tiles) so when you search for them you will get many paths to the world root
	// you will need to search these paths to find the one that goes through the tile you need
	// this will give you a matix transfor for the animation point which you will need to decompose into 
	// a position (vec3) and rotation (quat)
}

raaAnimationPointFinder::~raaAnimationPointFinder()
{
}

osg::MatrixTransform* raaAnimationPointFinder::animationPoint()
{
	osg::Group* tileFinder = raaFinder<osg::Group>(m_sTile, m_pRoot).node();
	osg::MatrixTransform* animationPointFinder = raaFinder<osg::MatrixTransform>(std::to_string(m_uiAnimPoint), tileFinder).node();

	return animationPointFinder;
}

const raaAnimationPointFinder& raaAnimationPointFinder::operator=(const raaAnimationPointFinder& ap)
{
	m_sTile = ap.m_sTile;
	m_uiAnimPoint = ap.m_uiAnimPoint;
	m_pRoot = ap.m_pRoot;
	return ap;
}


int raaAnimationPointFinder::nodePathFinder()
{
	osg::NodePathList nodePathList = animationPoint()->getParentalNodePaths(m_pRoot);
	osg::NodePathList::iterator findInterator;
	int count = 0;

	{
		for (osg::NodePathList::iterator iterator = nodePathList.begin(); iterator != nodePathList.end(); iterator++)
		{
			osg::NodePath nodePath;
			bool tile = false;
			bool animPoint = false;

			for (int i = 0; i < (*iterator).size(); i++)
			{

				std::string nodePathString = (*iterator)[i]->getName();
				if (nodePathString.compare(m_sTile) == 0)
				{
					tile = true;
				}
				if (nodePathString.compare(std::to_string(m_uiAnimPoint)) == 0)
				{
					animPoint = true;
				}
				if (tile && animPoint)
				{
					return count;
				}
			}
			count++;
		}
		return 0;
	}
}



osg::Vec3 raaAnimationPointFinder::translation()
{
	return osg::computeLocalToWorld(animationPoint()->getParentalNodePaths(m_pRoot)[nodePathFinder()]).getTrans();
}

osg::Quat raaAnimationPointFinder::rotation()
{
	return osg::computeLocalToWorld(animationPoint()->getParentalNodePaths(m_pRoot)[nodePathFinder()]).getRotate();
}
