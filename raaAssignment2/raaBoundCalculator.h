#pragma once

#include <windows.h>
#include <osg/NodeVisitor>

// a useful tool for caluating the bounding box of a subtree

class raaBoundCalculator: osg::NodeVisitor
{
public:
	raaBoundCalculator(osg::Node* pNode);
	raaBoundCalculator(osg::Node* pNode, osg::Geode* pTarget);
	virtual ~raaBoundCalculator();
	void apply(osg::Node& node) override;
	void apply(osg::Geode& node) override;
	void apply(osg::MatrixTransform& node) override;
	void apply(osg::Billboard& node) override;
	osg::BoundingBox& bound();
	float xDim();
	float yDim();
	float zDim();
	osg::Vec3f centre();
protected:
	osg::Matrix m_Transform;
	osg::BoundingBox m_BoundingBox;
	osg::Geode* m_pTarget;
};

