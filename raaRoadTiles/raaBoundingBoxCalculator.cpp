
#include <windows.h>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include "raaBoundingBoxCalculator.h"

raaBoundingBoxCalculator::raaBoundingBoxCalculator(osg::Node* pNode): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	m_Transform.makeIdentity();
	traverse(*pNode);
}

raaBoundingBoxCalculator::~raaBoundingBoxCalculator()
{
}

void raaBoundingBoxCalculator::apply(osg::Node& node)
{
	traverse(node);
}

void raaBoundingBoxCalculator::apply(osg::Geode& node)
{
	osg::BoundingBox bb, bbt;

	for(unsigned int i=0;i<node.getNumDrawables();i++) bb.expandBy(node.getDrawable(i)->getBoundingBox());
	for (unsigned int i = 0; i < 8; i++) bbt.expandBy(bb.corner(i) * m_Transform);

	m_BoundingBox.expandBy(bbt);
	
	traverse(node);
}

void raaBoundingBoxCalculator::apply(osg::MatrixTransform& node)
{
	m_Transform *= node.getMatrix();
	traverse(node);
}

osg::BoundingBox& raaBoundingBoxCalculator::bound()
{
	return m_BoundingBox;
}

float raaBoundingBoxCalculator::xDim()
{
	return m_BoundingBox.xMax() - m_BoundingBox.xMin();
}

float raaBoundingBoxCalculator::yDim()
{
	return m_BoundingBox.yMax() - m_BoundingBox.yMin();
}

float raaBoundingBoxCalculator::zDim()
{
	return m_BoundingBox.zMax() - m_BoundingBox.zMin();
}
