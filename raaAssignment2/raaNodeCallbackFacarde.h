#pragma once

#include <windows.h>
#include <osg/NodeCallback>
#include "raaFacarde.h"

class raaNodeCallbackFacarde: public raaFacarde, public osg::NodeCallback
{
public:
	raaNodeCallbackFacarde(osg::Node* pPart);
	raaNodeCallbackFacarde(osg::Node* pPart, osg::Vec3 vTrans);
	raaNodeCallbackFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot);
	raaNodeCallbackFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale);
	virtual ~raaNodeCallbackFacarde();
	
	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
};

