#include "raaNodeCallbackFacarde.h"

raaNodeCallbackFacarde::raaNodeCallbackFacarde(osg::Node* pPart): raaFacarde(pPart)
{
	if (m_pRoot) m_pRoot->addUpdateCallback(this);
}

raaNodeCallbackFacarde::raaNodeCallbackFacarde(osg::Node* pPart, osg::Vec3 vTrans) : raaFacarde(pPart, vTrans)
{
	if (m_pRoot) m_pRoot->addUpdateCallback(this);
}

raaNodeCallbackFacarde::raaNodeCallbackFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot) : raaFacarde(pPart, vTrans, fRot)
{
	if (m_pRoot) m_pRoot->addUpdateCallback(this);
}

raaNodeCallbackFacarde::raaNodeCallbackFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale) : raaFacarde(pPart, vTrans, fRot, fScale)
{
	if (m_pRoot) m_pRoot->addUpdateCallback(this);
}

raaNodeCallbackFacarde::~raaNodeCallbackFacarde()
{
	if (m_pRoot) m_pRoot->removeUpdateCallback(this);
}

void raaNodeCallbackFacarde::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	nv->traverse(*node);
}
