#include <windows.h>
#include <osg/Switch>
#include "raaSwitchControl.h"

raaSwitchControl::raaSwitchControl(osg::Node* pNode, bool bOn): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), m_bOn(bOn)
{
	traverse(*pNode);
}

raaSwitchControl::~raaSwitchControl()
{
}

void raaSwitchControl::apply(osg::Switch& node)
{
	if (m_bOn) node.setAllChildrenOn();
	else node.setAllChildrenOff();

	traverse(node);

}
