#pragma once

#include <windows.h>
#include <osg/NodeVisitor>

class raaSwitchActivator: public osg::NodeVisitor
{
public:
	raaSwitchActivator(std::string sName, osg::Node* pNode, bool bOn = true);
	virtual ~raaSwitchActivator();


	void apply(osg::Switch& node) override;
protected:
	bool m_bOn;
	std::string m_sName;
};

