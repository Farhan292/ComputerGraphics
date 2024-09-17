#pragma once
#include <windows.h>
#include <map>
#include <osg/Node>
#include <osg/Group>
#include <string>

typedef  std::map<std::string, osg::Node*> raaAssetList;

class raaAssetLibrary
{
public:
	static void start();
	static void stop();

	static bool loadAsset(std::string sName, std::string sFile);
	static osg::Node* getNamedAsset(std::string sName, std::string sUniquePartName);
	static osg::Node* getClonedAsset(std::string sName, std::string sUniquePartName);
	static bool insertAsset(std::string sName, osg::Node* pNode);
	static osg::Node* getAssetsRoot();
	
	static osg::Node* asset(std::string sName);
protected:
	raaAssetLibrary();
	virtual ~raaAssetLibrary();

private:
	static raaAssetLibrary* sm_pInstance;

	static osg::Group* sm_pAssetRoot;
	
	raaAssetList m_Assets;
};


