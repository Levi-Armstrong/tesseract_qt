#include <tesseract_qt/rendering/ogre2/ogre2_widget.h>
#include <tesseract_qt/rendering/ogre2/ogre2_renderman.h>
#include <tesseract_qt/rendering/ogre2/ogre2_constants.h>

#include <QApplication>
#include <memory>

#include "ogre2_custom_widget.h"

int main(int argc, char* argv[])
{
  // Initialize the Qt Application
  QApplication a(argc, argv);

  auto ogre_manager = new tesseract_gui::OgreManager();
  ogre_manager->initialize();

  auto ogre_widget = new tesseract_gui::QOgreWidget();
  //    ogre_widget->setRenderwindowDockWidget(this);
  ogre_manager->registerOgreWidget(tesseract_gui::OGRE_WIDGET_RENDERWINDOW, ogre_widget);
  ogre_widget->createRenderWindow(ogre_manager);
  ogre_widget->show();

  //    //Create the widget
  //    tesseract_gui::Ogre2CustomWidget widget; widget.show();
  //    auto w = widget.w;
  //    auto w2 = widget.w2;

  //    //Resource locations
  //    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/home/larmstrong/catkin_ws/tesseract_ws/src/tesseract_qt/rendering/ogre2/demo",
  //    "FileSystem");

  //    //HighLevelMaterialSystem shader libraries
  //    qtOgre.registerHlms();

  //    //Initialize the resources
  //    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);

  //    //Setup 1st scene (that is in 1st widget
  //    auto scene = qtOgre.getScene(0);
  //    auto knotNode = scene->getRootSceneNode()->createChildSceneNode();
  //    auto knotEntity = scene->createEntity("knot.mesh");
  //    knotNode->attachObject(knotEntity);
  //    auto sunlight = scene->createLight();
  //    auto sunNode = scene->getRootSceneNode()->createChildSceneNode();
  //    sunNode->attachObject(sunlight);
  //    sunlight->setType(Ogre::Light::LT_DIRECTIONAL);
  //    sunlight->setDirection(Ogre::Vector3(-1, -1, -0.5f).normalisedCopy());
  //    sunlight->setPowerScale(Ogre::Math::PI);
  //    auto Camera = w->getCamera();
  //    Camera->setPosition(0, 0, 300.5f);
  //    Camera->setAutoAspectRatio(true);

  //    //Setup 2nd scene (2nd widget)
  //    auto scene2 = qtOgre.getScene(1);
  //    auto knotNode2 = scene2->getRootSceneNode()->createChildSceneNode();
  //    auto knotEntity2 = scene2->createEntity("knot.mesh");
  //    knotNode2->attachObject(knotEntity2);
  ////    knotNode2->setScale(0.01f, 0.01f, 0.01f);
  //    auto sunlight2 = scene2->createLight();
  //    auto sunNode2 = scene2->getRootSceneNode()->createChildSceneNode();
  //    sunNode2->attachObject(sunlight2);
  //    sunlight2->setType(Ogre::Light::LT_DIRECTIONAL);
  //    sunlight2->setDirection(Ogre::Vector3(-1, -1, -0.5f).normalisedCopy());
  //    sunlight2->setPowerScale(Ogre::Math::PI);
  //    auto Camera2 = w2->getCamera();
  //    Camera2->setPosition(0, 0, 300.5f);
  ////    Camera2->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),
  ////                                             Ogre::Vector3::UNIT_X));
  //    Camera2->setAutoAspectRatio(true);

  return QApplication::exec();
}
