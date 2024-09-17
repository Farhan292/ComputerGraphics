#include <windows.h>
#include <osg/Switch>
#include "raaSwitchActivator.h"

raaSwitchActivator::raaSwitchActivator(std::string sName, osg::Node* pNode, bool bOn): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), m_bOn(bOn), m_sName(sName)
{
	if (pNode) traverse(*pNode);
		
}

raaSwitchActivator::~raaSwitchActivator()
{
}

void raaSwitchActivator::apply(osg::Switch& node)
{
	if(node.getName()==m_sName)
	{
		if (m_bOn) node.setAllChildrenOn();
		else node.setAllChildrenOff();
	}
	
	traverse(node);
}
