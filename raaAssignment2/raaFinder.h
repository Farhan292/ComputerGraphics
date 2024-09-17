#pragma once

// note: this is a templated class so implemented in the header file
#include <windows.h>
#include <osg/NodeVisitor>

// a generic finder implmentation for you to use

template<class T>
class raaFinder : public osg::NodeVisitor
{
public:
	raaFinder(std::string sName, osg::Node* pNode=0);

	virtual ~raaFinder();

	T* node();

	virtual void apply(osg::Node& node);

protected:
	std::string m_sName;
	T *m_pNode;
};

template <class T>
raaFinder<T>::raaFinder(std::string sName, osg::Node* pNode): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN), m_sName(sName), m_pNode(0)
{
	if(pNode) apply(*pNode);
}

template <class T>
raaFinder<T>::~raaFinder() 
{
	if (m_pNode) m_pNode->unref();
}

template <class T>
T* raaFinder<T>::node() 
{
	return m_pNode;
}

template <class T>
void raaFinder<T>::apply(osg::Node& node) 
{
	if (dynamic_cast<T*>(&node) && node.getName() == m_sName)
	{
		m_pNode = dynamic_cast<T*>(&node);
		m_pNode->ref();
	}
	else
		traverse(node);
}
