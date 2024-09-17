
#include <windows.h>
#include <osg/Billboard>
#include <osgText/Text>
#include <osg/Switch>
#include "raaBoundCalculator.h"
#include "raaFinder.h"
#include "raaFacarde.h"

raaFacardes raaFacarde::sm_lFacardes;
bool raaFacarde::sm_bShowNames = false;

raaFacarde::raaFacarde(osg::Node* pPart) : m_pTranslation(0), m_pRotation(0), m_pScale(0), m_pRoot(pPart)
{
	init();
}

raaFacarde::raaFacarde(osg::Node* pPart, osg::Vec3 vTrans) : m_pTranslation(0), m_pRotation(0), m_pScale(0), m_pRoot(pPart)
{
	init();

	osg::Matrix m;
	m.makeTranslate(vTrans);
	if (m_pTranslation) m_pTranslation->setMatrix(m);
}

raaFacarde::raaFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot) : m_pTranslation(0), m_pRotation(0), m_pScale(0), m_pRoot(pPart)
{
	init();

	osg::Matrix m;
	m.makeTranslate(vTrans);
	if (m_pTranslation) m_pTranslation->setMatrix(m);
	m.makeRotate(osg::DegreesToRadians(fRot), osg::Vec3(0.0f, 0.0f, 1.0f));
	if (m_pRotation) m_pRotation->setMatrix(m);
}

raaFacarde::raaFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale) : m_pTranslation(0), m_pRotation(0), m_pScale(0), m_pRoot(pPart)
{
	init();

	osg::Matrix m;
	m.makeTranslate(vTrans);
	if (m_pTranslation) m_pTranslation->setMatrix(m);
	m.makeRotate(osg::DegreesToRadians(fRot), osg::Vec3(0.0f, 0.0f, 1.0f));
	if (m_pRotation) m_pRotation->setMatrix(m);
	m.makeScale(fScale, fScale, fScale);
	if (m_pScale) m_pScale->setMatrix(m);
}

raaFacarde::~raaFacarde()
{
	sm_lFacardes.remove(this);
	if (m_pRoot) m_pRoot->unref();
}

osg::MatrixTransform* raaFacarde::translation()
{
	return m_pTranslation;
}

osg::MatrixTransform* raaFacarde::rotation()
{
	return m_pRotation;
}

osg::Node* raaFacarde::root()
{
	return m_pRoot;
}

void raaFacarde::showName(bool bShow)
{
	if(m_pNameSwitch)
	{
		if (bShow) m_pNameSwitch->setAllChildrenOn();
		else m_pNameSwitch->setAllChildrenOff();
	}
}

void raaFacarde::showNames(bool bShow)
{
	for (raaFacardes::iterator it = sm_lFacardes.begin(); it != sm_lFacardes.end(); it++) (*it)->showName(bShow);
}

void raaFacarde::toggleNames()
{
	sm_bShowNames = !sm_bShowNames;
	for (raaFacardes::iterator it = sm_lFacardes.begin(); it != sm_lFacardes.end(); it++) (*it)->showName(sm_bShowNames);
}

const raaFacardes& raaFacarde::facardes()
{
	return sm_lFacardes;
}

void raaFacarde::init()
{
	if (m_pRoot)
	{
		m_pRoot->ref();
		raaFinder<osg::MatrixTransform> t("Translation", m_pRoot);
		m_pTranslation = t.node();
		raaFinder<osg::MatrixTransform> r("Rotation", m_pRoot);
		m_pRotation = r.node();
		raaFinder<osg::MatrixTransform> s("Scale", m_pRoot);
		m_pScale = s.node();

		raaBoundCalculator bbc(m_pRoot);

		m_pNameSwitch = new osg::Switch();
		osg::Billboard* pNameBillboard = new osg::Billboard();
		osgText::Text* pText = new osgText::Text();
		pText->setText(m_pRoot->getName());
		pText->setColor(osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
		pText->setAlignment(osgText::Text::AlignmentType::CENTER_CENTER);
		pNameBillboard->addDrawable(pText);
		pNameBillboard->setMode(osg::Billboard::POINT_ROT_EYE);
		pNameBillboard->setPosition(0, osg::Vec3f(0.0f, 0.0f, 0.2 * bbc.xDim()));
		pNameBillboard->setNormal(osg::Vec3f(0.0f, 0.0f, 1.0f));
		m_pNameSwitch->addChild(pNameBillboard);
		m_pRotation->addChild(m_pNameSwitch);

		if (sm_bShowNames) m_pNameSwitch->setAllChildrenOn();
		else m_pNameSwitch->setAllChildrenOff();
	}

	sm_lFacardes.push_back(this);
}
osg::Vec3f raaFacarde::getWorldDetectionPoint()
{
	//return osg::Vec3(); // should return the world position of the detection point for this subtree
	return this->m_pRoot->getBound().center(); // should return the world position of the detection point for this subtree
}

osg::Vec3f raaFacarde::getWorldCollisionPoint()
{
	return osg::Vec3(); // should return the world position of the collision point for this subtree
}

