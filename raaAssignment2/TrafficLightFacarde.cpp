#include "TrafficLightFacarde.h"
#include "raaFinder.h"

TrafficLightFacarde::TrafficLightFacarde(osg::Node* pPart, osg::Vec3 vTrans, float fRot, float fScale) :raaFacarde(pPart, vTrans, fRot, fScale)
{
	initLights(pPart);
}

TrafficLightFacarde::~TrafficLightFacarde()
{

}

void TrafficLightFacarde::initLights(osg::Node* pPart)
{
	raaFinder<osg::Geode>redLightFinder("trafficLight::RedLamp-GEODE", pPart);
	raaFinder<osg::Geode>amberLightFinder("trafficLight::AmberLamp-GEODE", pPart);
	raaFinder<osg::Geode>greenLightFinder("trafficLight::GreenLamp-GEODE", pPart);

	m_pRedTrafficLight = redLightFinder.node();
	m_pAmberTrafficLight = amberLightFinder.node();
	m_pGreenTrafficLight = greenLightFinder.node();

	m_pRedTrafficLightOnMaterial = new osg::Material();
	m_pRedTrafficLightOnMaterial->ref();
	m_pRedTrafficLightOffMaterial = new osg::Material();
	m_pRedTrafficLightOffMaterial->ref();

	m_pAmberTrafficLightOnMaterial = new osg::Material();
	m_pAmberTrafficLightOnMaterial->ref();
	m_pAmberTrafficLightOffMaterial = new osg::Material();
	m_pAmberTrafficLightOffMaterial->ref();

	m_pGreenTrafficLightOnMaterial = new osg::Material();
	m_pGreenTrafficLightOnMaterial->ref();
	m_pGreenTrafficLightOffMaterial = new osg::Material();
	m_pGreenTrafficLightOffMaterial->ref();

	createMaterial({ 0.85f, 0.0f, 0.0f }, m_pRedTrafficLightOnMaterial);
	createMaterial({ 0.09f, 0.0f, 0.0f }, m_pRedTrafficLightOffMaterial);
	createMaterial({ 1.0f, 0.7f, 0.0f }, m_pAmberTrafficLightOnMaterial);
	createMaterial({ 0.15f, 0.11f, 0.0f }, m_pAmberTrafficLightOffMaterial);
	createMaterial({ 0.0f, 0.85f, 0.0f }, m_pGreenTrafficLightOnMaterial);
	createMaterial({ 0.0f, 0.09f, 0.0f }, m_pGreenTrafficLightOffMaterial);

	setRedTrafficLight();
}

void TrafficLightFacarde::createMaterial(osg::Vec3f vColour, osg::Material* mat)
{
	mat->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(vColour[0], vColour[1], vColour[2], 1.0f));
	mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(vColour[0], vColour[1], vColour[2], 1.0f));
	mat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(vColour[0], vColour[1], vColour[2], 1.0f));
}

void TrafficLightFacarde::setRedTrafficLight()
{
	m_iTrafficLightStatus = 1;

	m_pRedTrafficLight->getOrCreateStateSet()->setAttribute(m_pRedTrafficLightOnMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	m_pAmberTrafficLight->getOrCreateStateSet()->setAttribute(m_pAmberTrafficLightOffMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	m_pGreenTrafficLight->getOrCreateStateSet()->setAttribute(m_pGreenTrafficLightOffMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
}

void TrafficLightFacarde::setAmberTrafficLight()
{
	m_iTrafficLightStatus = 2;

	m_pRedTrafficLight->getOrCreateStateSet()->setAttribute(m_pRedTrafficLightOffMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	m_pAmberTrafficLight->getOrCreateStateSet()->setAttribute(m_pAmberTrafficLightOnMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	m_pGreenTrafficLight->getOrCreateStateSet()->setAttribute(m_pGreenTrafficLightOffMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
}

void TrafficLightFacarde::setGreenTrafficLight()
{
	m_iTrafficLightStatus = 3;

	m_pRedTrafficLight->getOrCreateStateSet()->setAttribute(m_pRedTrafficLightOffMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	m_pAmberTrafficLight->getOrCreateStateSet()->setAttribute(m_pAmberTrafficLightOffMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
	m_pGreenTrafficLight->getOrCreateStateSet()->setAttribute(m_pGreenTrafficLightOnMaterial, osg::StateAttribute::ON || osg::StateAttribute::OVERRIDE);
}
int TrafficLightFacarde::getCurrentTrafficLightStatus() const
{
	return m_iTrafficLightStatus;
}

