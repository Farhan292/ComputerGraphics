#pragma once

#include <windows.h>
#include <osg/NodeVisitor>

class raaBoundingBoxCalculator: public osg::NodeVisitor
{
public:
	raaBoundingBoxCalculator(osg::Node *pNode);
	virtual ~raaBoundingBoxCalculator();
	void apply(osg::Node& node) override;
	void apply(osg::Geode& node) override;
	void apply(osg::MatrixTransform& node) override;
	osg::BoundingBox& bound();
	float xDim();
	float yDim();
	float zDim();
protected:
	osg::Matrix m_Transform;
	osg::BoundingBox m_BoundingBox;
};


