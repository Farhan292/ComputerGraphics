#pragma once
#include <windows.h>
#include <osg/MatrixTransform>
#include "raaCollisionTarget.h"

// the base class of all facardes
// this adds 3 transforms to control translation, location and scale to the asset (which may be instanced or clones) allowing unique poition, rot, scale for this instance

typedef std::list<class raaFacarde*> raaFacardes;

class raaFacarde
{
public:
	raaFacarde(osg::Node* pPart);
	raaFacarde(osg::Node* pPart, osg::Vec3 vTrans);
	raaFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot);
	raaFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale);
	virtual ~raaFacarde();

	osg::MatrixTransform* translation();
	osg::MatrixTransform* rotation();
	osg::MatrixTransform* scale();
	osg::Node* root();
	void showName(bool bShow);
	static void showNames(bool bShow);
	static void toggleNames();
	
	virtual osg::Vec3f getWorldDetectionPoint();
	virtual osg::Vec3f getWorldCollisionPoint();

	static const raaFacardes& facardes();
	static const raaFacardes& getFacardes()
	{
		return sm_lFacardes;
	}

protected:
	void init();
	
	osg::MatrixTransform* m_pTranslation;
	osg::MatrixTransform* m_pRotation;
	osg::MatrixTransform* m_pScale;
	osg::Node* m_pRoot;
	osg::Switch* m_pNameSwitch;
	static raaFacardes sm_lFacardes;
	static bool sm_bShowNames;
};

