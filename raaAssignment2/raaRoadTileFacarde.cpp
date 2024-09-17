
#include <windows.h>
#include <osg/Switch>

#include "raaRoadTileFacarde.h"



raaRoadTileFacardesStringMap raaRoadTileFacarde::sm_mRoadTiles;


raaRoadTileFacarde::raaRoadTileFacarde(osg::Node* pPart): raaNodeCallbackFacarde(pPart)
{
	if(pPart) sm_mRoadTiles[pPart->getName()] = this;
}

raaRoadTileFacarde::raaRoadTileFacarde(osg::Node* pPart, osg::Vec3 vTrans) : raaNodeCallbackFacarde(pPart, vTrans)
{
	if (pPart) sm_mRoadTiles[pPart->getName()] = this;
}

raaRoadTileFacarde::raaRoadTileFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot) : raaNodeCallbackFacarde(pPart, vTrans, fRot)
{
	if (pPart) sm_mRoadTiles[pPart->getName()] = this;
}

raaRoadTileFacarde::raaRoadTileFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale) : raaNodeCallbackFacarde(pPart, vTrans, fRot, fScale)
{
	if (pPart) sm_mRoadTiles[pPart->getName()] = this;
}

raaRoadTileFacarde::~raaRoadTileFacarde()
{
	sm_mRoadTiles.erase(m_pRoot->getName());
}

const raaRoadTileFacardesStringMap& raaRoadTileFacarde::roadTiles()
{
	return sm_mRoadTiles;
}

