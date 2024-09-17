#include <windows.h>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/Switch>
#include <vector>
#include "raaPrinter.h"
#include "raaFinder.h"
#include "raaBoundingBoxCalculator.h"
#include "raaAnimPoint.h"
#include "raaSwitchControl.h"
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgDB/WriteFile>

typedef std::vector<raaAnimPoint> raaAnimPoints;

osg::Group* g_pSource = 0;
osg::Group* g_pRoot = 0;

osg::Group* buildTile(osg::MatrixTransform *pModel, std::string sName, raaAnimPoints& animPoints)
{
	osg::Group* pGroup = 0;

	if (pModel)
	{
		raaBoundingBoxCalculator bbc(pModel);

		std::cout << sName << " [" << bbc.xDim() << ", " << bbc.yDim() << ", " << bbc.zDim() << "]" << std::endl;
		
		pGroup = new osg::Group();
		pGroup->setName(sName);
		pGroup->setUserValue("width", bbc.xDim());
		pGroup->setUserValue("depth", bbc.yDim());


		osg::Switch* pTileIDS = new osg::Switch();
		pTileIDS->setName("IDSwitch");
		pTileIDS->setAllChildrenOn();

		osg::Switch* pAnimPointS = new osg::Switch();
		pAnimPointS->setName("AnimationPointSwitch");
		pAnimPointS->setAllChildrenOn();

		osg::Switch* pAnimIDS = new osg::Switch();
		pAnimIDS->setName("AnimationIDSwitch");
		pAnimIDS->setAllChildrenOn();

		pGroup->addChild(pModel);
		pGroup->addChild(pTileIDS);
		pGroup->addChild(pAnimPointS);
		pGroup->addChild(pAnimIDS);

		osg::Billboard* pTileBillboard = new osg::Billboard();
		osgText::Text* pText = new osgText::Text();
		std::stringstream ss;
		ss << sName;
		pText->setText(ss.str());
		pText->setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
		pText->setAlignment(osgText::Text::AlignmentType::CENTER_CENTER);
		pTileBillboard->addDrawable(pText);
		pTileBillboard->setMode(osg::Billboard::POINT_ROT_EYE);
		pTileBillboard->setPosition(0, osg::Vec3f(0.0f, 0.0f, 0.1f * bbc.xDim()));
		pTileBillboard->setNormal(osg::Vec3f(0.0f, 0.0f, 1.0f));
		pTileIDS->addChild(pTileBillboard);

		osg::Geode* pAnimGeode = new osg::Geode();
		osg::Cone* pCone = new osg::Cone(osg::Vec3f(-10.0f, 0.0f, 0.0f), 7.0f, 40.0f);
		pCone->setRotation(osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 1.0f, 0.0f)));

		osg::ShapeDrawable* pSD = new osg::ShapeDrawable(pCone);

		osg::Material* pMat = new osg::Material();
		pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(0.2f, 0.0f, 0.0f, 1.0f));
		pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.8f, 0.1f, 0.1f, 1.0f));
		pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		pMat->setShininess(osg::Material::FRONT, 180.0f);
		pAnimGeode->addDrawable(pSD);

		pAnimGeode->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

		osg::Billboard* pBillboard = new osg::Billboard();
		pAnimIDS->addChild(pBillboard);

		for (unsigned int uiIndex=0;uiIndex<animPoints.size();uiIndex++)
		{
			osg::Vec3 vPos = osg::Vec3(bbc.xDim()*animPoints[uiIndex].pos()[0], bbc.yDim()*animPoints[uiIndex].pos()[1], 0.0f);
			std::stringstream ss;
			ss << uiIndex;

			osg::MatrixTransform* pMT = new osg::MatrixTransform();
			pMT->setMatrix(osg::Matrixf::rotate(animPoints[uiIndex].rot()) * osg::Matrixf::translate(vPos));
			pMT->setName(ss.str());
			pMT->addChild(pAnimGeode);
			pAnimPointS->addChild(pMT);

			osgText::Text* pText = new osgText::Text();
			pText->setCharacterSize(10.0f);
	
			pText->setText(ss.str());
			pText->setAlignment(osgText::Text::AlignmentType::CENTER_CENTER);

			pBillboard->addDrawable(pText);
			pBillboard->setPosition(uiIndex, vPos + osg::Vec3f(0.0f, 0.0f, 20.0f));
		}
		
		
		osg::Matrix m = pModel->getMatrix();
		m.setTrans(0.0f, 0.0f, 0.0f);
		pModel->setMatrix(m);
		pModel->setName("offset");
	}

	return pGroup;
}


int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;

	g_pRoot = new osg::Group();
	g_pRoot->ref();
	g_pSource = new osg::Group();
	g_pSource->ref();

	// parse command line options
	std::string sRoadAsset = "../../Data/RoadSet.OSGB"; // imports the road assets
	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-ra") sRoadAsset = (argv[++i]);
	}

	g_pSource->addChild(osgDB::readNodeFile(sRoadAsset));

//	raaPrinter p;
//	p.traverse(*g_pSource);

	raaAnimPoints straightAP;
	straightAP.push_back(raaAnimPoint(osg::Vec3f(0.45f, 0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	straightAP.push_back(raaAnimPoint(osg::Vec3f(0.45f, -0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	straightAP.push_back(raaAnimPoint(osg::Vec3f(-0.45f, 0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	straightAP.push_back(raaAnimPoint(osg::Vec3f(-0.45f, -0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));

	raaAnimPoints curveAP;
	curveAP.push_back(raaAnimPoint(osg::Vec3f(0.12f, 0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	curveAP.push_back(raaAnimPoint(osg::Vec3f(0.24f, 0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	curveAP.push_back(raaAnimPoint(osg::Vec3f(0.45f, 0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	curveAP.push_back(raaAnimPoint(osg::Vec3f(0.45f, -0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	curveAP.push_back(raaAnimPoint(osg::Vec3f(0.06f, 0.06f, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	curveAP.push_back(raaAnimPoint(osg::Vec3f(-0.12f, 0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));

	raaAnimPoints tjunctionAP;
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.45f, 0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.24f, 0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.12f, 0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.02f, 0.02f, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.12f, -0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.12f, 0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.02f, 0.02f, 0.0f), osg::Quat(osg::DegreesToRadians(-135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.45f, -0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.12f, -0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));
	tjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.24f, -0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))));

	raaAnimPoints xjunctionAP;
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.45f, 0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //0
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.24f, 0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //1
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.12f, 0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //2
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.02f, 0.02f, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //3
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.12f, -0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //4
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.12f, 0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //5
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.02f, 0.02f, 0.0f), osg::Quat(osg::DegreesToRadians(-135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //6
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.45f, -0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //7
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.12f, -0.45f, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //8
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.24f, -0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //9
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.45f, 0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //10
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.45f, -0.12f, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //11
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.24f, -0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(-135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //12
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(-0.02f, -0.02f, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //13
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.24f, 0.24f, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //14
	xjunctionAP.push_back(raaAnimPoint(osg::Vec3f(0.02f, -0.02f, 0.0f), osg::Quat(osg::DegreesToRadians(45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)))); //15
	
	raaFinder<osg::MatrixTransform> findS("2Lanes", g_pSource);
	raaFinder<osg::MatrixTransform> findT("2LanesTIntersection", g_pSource);
	raaFinder<osg::MatrixTransform> findX("2LanesXIntersection", g_pSource);
	raaFinder<osg::MatrixTransform> findC("2LanesTurn", g_pSource);

	osg::Group* pGS = buildTile(findS.node(), "straight", straightAP);
	osg::Group* pGT = buildTile(findT.node(), "tJunction", tjunctionAP);
	osg::Group* pGX = buildTile(findX.node(), "xJunction", xjunctionAP);
	osg::Group* pGC = buildTile(findC.node(), "curve", curveAP);

//	raaPrinter pp;
//	pp.traverse(*pGS);

	raaSwitchControl scs(pGS, false);
	raaSwitchControl sct(pGT, false);
	raaSwitchControl scx(pGX, false);
	raaSwitchControl scc(pGC, false);

	
	osgDB::writeNodeFile(*pGS, "../../data/roadStraight.osg");
	osgDB::writeNodeFile(*pGT, "../../data/roadTJunction.osg");
	osgDB::writeNodeFile(*pGX, "../../data/roadXJunction.osg");
	osgDB::writeNodeFile(*pGC, "../../data/roadCurve.osg");

	raaSwitchControl scss(pGS);
	raaSwitchControl sctt(pGT);
	raaSwitchControl scxx(pGX);
	raaSwitchControl sccc(pGC);




	
//	raaFinder<osg::MatrixTransform> findMS("Translation", pGS);
//	raaFinder<osg::MatrixTransform> findMT("Translation", pGT);
//	raaFinder<osg::MatrixTransform> findMX("Translation", pGX);
//	raaFinder<osg::MatrixTransform> findMC("Translation", pGC);

	osg::MatrixTransform* pTransS = new osg::MatrixTransform();
	pTransS->setName("Translation");
	osg::MatrixTransform* pRotS = new osg::MatrixTransform();
	pRotS->setName("Rotation");
	pTransS->addChild(pRotS);
	pRotS->addChild(pGS);
	osg::Matrix mS;
	mS.setTrans(300.0f, 300.0f, 0.0f);
	pTransS->setMatrix(mS);
	g_pRoot->addChild(pTransS);

	osg::MatrixTransform* pTransX = new osg::MatrixTransform();
	pTransX->setName("Translation");
	osg::MatrixTransform* pRotX = new osg::MatrixTransform();
	pRotX->setName("Rotation");
	pTransX->addChild(pRotX);
	pRotX->addChild(pGX);
	osg::Matrix mX;
	mX.setTrans(-300.0f, 300.0f, 0.0f);
	pTransX->setMatrix(mX);
	g_pRoot->addChild(pTransX);

	osg::MatrixTransform* pTransT = new osg::MatrixTransform();
	pTransT->setName("Translation");
	osg::MatrixTransform* pRotT = new osg::MatrixTransform();
	pRotT->setName("Rotation");
	pTransT->addChild(pRotT);
	pRotT->addChild(pGT);
	osg::Matrix mT;
	mT.setTrans(-300.0f, -300.0f, 0.0f);
	pTransT->setMatrix(mT);
	g_pRoot->addChild(pTransT);

	osg::MatrixTransform* pTransC = new osg::MatrixTransform();
	pTransC->setName("Translation");
	osg::MatrixTransform* pRotC = new osg::MatrixTransform();
	pRotC->setName("Rotation");
	pTransC->addChild(pRotC);
	pRotC->addChild(pGC);
	osg::Matrix mC;
	mC.setTrans(300.0f,-300.0f, 0.0f);
	pTransC->setMatrix(mC);
	g_pRoot->addChild(pTransC);

	
	// setup stuff
	osg::GraphicsContext::Traits* pTraits = new osg::GraphicsContext::Traits();
	pTraits->x = 20;
	pTraits->y = 20;
	pTraits->width = 600;
	pTraits->height = 480;
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = 0;

	osg::GraphicsContext* pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits);
	osgGA::KeySwitchMatrixManipulator* pKeyswitchManipulator = new osgGA::KeySwitchMatrixManipulator();
	pKeyswitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
	pKeyswitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator());
	pKeyswitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator());
	viewer.setCameraManipulator(pKeyswitchManipulator);
	osg::Camera* pCamera = viewer.getCamera();
	pCamera->setGraphicsContext(pGraphicsContext);
	pCamera->setViewport(new osg::Viewport(0, 0, pTraits->width, pTraits->height));


	// add the state manipulator
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

	// add the thread model handler
	viewer.addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	viewer.addEventHandler(new osgViewer::StatsHandler);

	// add the record camera path handler
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	viewer.addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

	// set the scene to render
	viewer.setSceneData(g_pRoot);

	viewer.realize();

	return viewer.run();
}


