#pragma once

#include <windows.h>
#include <osg/NodeVisitor>

class raaSwitchControl: public osg::NodeVisitor
{
public:
	raaSwitchControl(osg::Node* pNode, bool bOn = true);
	virtual ~raaSwitchControl();
	
	void apply(osg::Switch& node) override;
protected:
	bool m_bOn;
};

