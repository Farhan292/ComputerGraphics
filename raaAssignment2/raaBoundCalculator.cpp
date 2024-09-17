#include <windows.h>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Billboard>

#include "raaBoundCalculator.h"

raaBoundCalculator::raaBoundCalculator(osg::Node* pNode) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), m_pTarget(0)
{
	m_Transform.makeIdentity();
	traverse(*pNode);
}

raaBoundCalculator::raaBoundCalculator(osg::Node* pNode, osg::Geode* pTarget) : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), m_pTarget(pTarget)
{
	m_Transform.makeIdentity();
	traverse(*pNode);
}

raaBoundCalculator::~raaBoundCalculator()
{
}

void raaBoundCalculator::apply(osg::Node& node)
{
	traverse(node);
}

void raaBoundCalculator::apply(osg::Geode& node)
{
	if(m_pTarget && m_pTarget==&node)
	{
		osg::BoundingBox bb, bbt;

		for (unsigned int i = 0; i < node.getNumDrawables(); i++) bb.expandBy(node.getDrawable(i)->getBoundingBox());
		for (unsigned int i = 0; i < 8; i++) bbt.expandBy(bb.corner(i) * m_Transform);

		m_BoundingBox.expandBy(bbt);
	}
	else
	{
		osg::BoundingBox bb, bbt;

		for (unsigned int i = 0; i < node.getNumDrawables(); i++) bb.expandBy(node.getDrawable(i)->getBoundingBox());
		for (unsigned int i = 0; i < 8; i++) bbt.expandBy(bb.corner(i) * m_Transform);

		m_BoundingBox.expandBy(bbt);
	}
	
	traverse(node);
}

void raaBoundCalculator::apply(osg::MatrixTransform& node)
{
	m_Transform *= node.getMatrix();
	traverse(node);
}

void raaBoundCalculator::apply(osg::Billboard& node)
{
	traverse(node);
}

osg::BoundingBox& raaBoundCalculator::bound()
{
	return m_BoundingBox;
}

float raaBoundCalculator::xDim()
{
	return m_BoundingBox.xMax() - m_BoundingBox.xMin();
}

float raaBoundCalculator::yDim()
{
	return m_BoundingBox.yMax() - m_BoundingBox.yMin();
}

float raaBoundCalculator::zDim()
{
	return m_BoundingBox.zMax() - m_BoundingBox.zMin();
}

osg::Vec3f raaBoundCalculator::centre()
{
	return m_BoundingBox.center();
}
