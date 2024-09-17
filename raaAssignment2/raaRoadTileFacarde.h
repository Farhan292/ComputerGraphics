#pragma once
#include <map>
#include "raaNodeCallbackFacarde.h"

typedef std::map<std::string, class raaRoadTileFacarde*> raaRoadTileFacardesStringMap;

class raaRoadTileFacarde: public raaNodeCallbackFacarde
{
public:
	raaRoadTileFacarde(osg::Node* pPart);
	raaRoadTileFacarde(osg::Node* pPart, osg::Vec3 vTrans);
	raaRoadTileFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot);
	raaRoadTileFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale);
	virtual ~raaRoadTileFacarde();

	static const raaRoadTileFacardesStringMap& roadTiles();
	
protected:
	static raaRoadTileFacardesStringMap sm_mRoadTiles;
};

