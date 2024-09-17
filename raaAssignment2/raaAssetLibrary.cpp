#include <windows.h>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>


#include "raaAssetLibrary.h"



raaAssetLibrary* raaAssetLibrary::sm_pInstance = 0;
osg::Group* raaAssetLibrary::sm_pAssetRoot=0;

void raaAssetLibrary::start()
{
	if (!sm_pAssetRoot) sm_pAssetRoot = new osg::Group();
	if (!sm_pInstance) new raaAssetLibrary();
	sm_pAssetRoot->ref();
}

void raaAssetLibrary::stop()
{
	if (sm_pInstance) delete sm_pInstance;
	if (sm_pAssetRoot) sm_pAssetRoot->unref();
}

bool raaAssetLibrary::loadAsset(std::string sName, std::string sFile)
{
	if (sm_pInstance && sName.length() && sFile.length())
	{
		sm_pInstance->m_Assets[sName] = osgDB::readNodeFile(sFile);

		if (sm_pInstance->m_Assets[sName])
		{
			sm_pInstance->m_Assets[sName]->setName(sName);
			sm_pAssetRoot->addChild(sm_pInstance->m_Assets[sName]);
			return true;
		}
		else sm_pInstance->m_Assets.erase(sName);
	}
	return false;
}

osg::Node* raaAssetLibrary::getNamedAsset(std::string sName, std::string sUniquePartName)
{
	if (sm_pInstance && sName.length() && sm_pInstance->m_Assets.find(sName) != sm_pInstance->m_Assets.end())
	{
		osg::Node* pNode = sm_pInstance->m_Assets[sName];
		if(pNode)
		{
			osg::Group* pGroup = new osg::Group();
			pGroup->setName(sUniquePartName);

			osg::MatrixTransform* pMT = new osg::MatrixTransform();
			pMT->setName("Translation");

			osg::MatrixTransform* pMR = new osg::MatrixTransform();
			pMR->setName("Rotation");

			osg::MatrixTransform* pMS = new osg::MatrixTransform();
			pMS->setName("Scale");

			pGroup->addChild(pMT);
			pMT->addChild(pMR);
			pMR->addChild(pMS);
			pMS->addChild(pNode);

			return pGroup;
		}
	}
	return 0;
}

osg::Node* raaAssetLibrary::getClonedAsset(std::string sName, std::string sUniquePartName)
{
	if (sm_pInstance && sName.length() && sm_pInstance->m_Assets.find(sName) != sm_pInstance->m_Assets.end())
	{
		osg::Node* pNode = (osg::Node*)sm_pInstance->m_Assets[sName]->clone(osg::CopyOp::DEEP_COPY_ALL);

		if (pNode)
		{
			osg::Group* pGroup = new osg::Group();
			pGroup->setName(sUniquePartName);

			osg::MatrixTransform* pMT = new osg::MatrixTransform();
			pMT->setName("Translation");

			osg::MatrixTransform* pMR = new osg::MatrixTransform();
			pMR->setName("Rotation");

			osg::MatrixTransform* pMS = new osg::MatrixTransform();
			pMS->setName("Scale");

			pGroup->addChild(pMT);
			pMT->addChild(pMR);
			pMR->addChild(pMS);
			pMS->addChild(pNode);

			return pGroup;
		}
	}
	return 0;
}

bool raaAssetLibrary::insertAsset(std::string sName, osg::Node* pNode)
{
	if (sm_pInstance && pNode)
	{
		sm_pInstance->m_Assets[sName] = pNode;

		if (sm_pInstance->m_Assets[sName])
		{
//			sm_pInstance->m_Assets[sName]->setName(sName);
			sm_pAssetRoot->addChild(sm_pInstance->m_Assets[sName]);
			return true;
		}
		else sm_pInstance->m_Assets.erase(sName);
	}
	return false;
}

osg::Node* raaAssetLibrary::asset(std::string sName)
{
	if (sm_pInstance && sName.length()) return sm_pInstance->m_Assets[sName];
	return 0;
}

raaAssetLibrary::raaAssetLibrary()
{
	if (!sm_pInstance) sm_pInstance = this;
}

raaAssetLibrary::~raaAssetLibrary()
{
	if (sm_pInstance == this) sm_pInstance = 0;
}

osg::Node* raaAssetLibrary::getAssetsRoot()
{
	return sm_pAssetRoot;
}
