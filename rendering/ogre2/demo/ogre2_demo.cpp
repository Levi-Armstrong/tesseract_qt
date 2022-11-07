#include <tesseract_qt/rendering/ogre2/ogre2_viewport.h>
#include <tesseract_qt/rendering/ogre2/ogre2_conversions.h>
#include <tesseract_support/tesseract_support_resource_locator.h>
#include <tesseract_environment/environment.h>

#include <QApplication>
#include <memory>

#include "ogre2_custom_widget.h"

int main(int argc, char* argv[])
{
  // Initialize the Qt Application
  QApplication a(argc, argv);

  auto locator = std::make_shared<tesseract_common::TesseractSupportResourceLocator>();
  tesseract_common::fs::path urdf_path =
      locator->locateResource("package://tesseract_support/urdf/lbr_iiwa_14_r820.urdf")->getFilePath();
  tesseract_common::fs::path srdf_path =
      locator->locateResource("package://tesseract_support/urdf/lbr_iiwa_14_r820.srdf")->getFilePath();
  auto env = std::make_shared<tesseract_environment::Environment>();
  if (!env->init(urdf_path, srdf_path, locator))
    exit(1);

  // Initialize the QtOgre system
  tesseract_gui::Ogre2Manager qtOgre(tesseract_gui::GraphicsAPI::OPENGL);

  // Create the widget
  tesseract_gui::Ogre2CustomWidget widget;
  widget.show();
  auto w = widget.w;
  auto w2 = widget.w2;

  // Resource locations
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/home/larmstrong/catkin_ws/tesseract_ws/src/"
                                                                 "tesseract_qt/rendering/ogre2/demo",
                                                                 "FileSystem");

  // HighLevelMaterialSystem shader libraries
  qtOgre.registerHlms();

  // Initialize the resources
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);

  auto entity_manager = std::make_shared<tesseract_gui::EntityManager>();
  Ogre::SceneManager* scene_manager = qtOgre.getScene(0);
  //  auto urdf_node = scene_manager->getRootSceneNode()->createChildSceneNode();
  //  tesseract_gui::loadSceneGraph(*scene_manager, *urdf_node, *entity_manager, *env->getSceneGraph());
  //   Setup 1st scene (that is in 1st widget
  //  Ogre::SceneManager* scene_manager = qtOgre.getScene(0);
  auto knotNode = scene_manager->getRootSceneNode()->createChildSceneNode();
  auto knotEntity = scene_manager->createEntity("knot.mesh");

  tesseract_scene_graph::Material green("green1234");
  green.color = Eigen::Vector4d(0, 0, 1, 1);
  knotEntity->setMaterial(tesseract_gui::loadMaterial(green));
  knotNode->attachObject(knotEntity);
  auto sunlight = scene_manager->createLight();
  auto sunNode = scene_manager->getRootSceneNode()->createChildSceneNode();
  sunNode->attachObject(sunlight);
  sunlight->setType(Ogre::Light::LT_DIRECTIONAL);
  sunlight->setDirection(Ogre::Vector3(-1, -1, -0.5f).normalisedCopy());
  sunlight->setPowerScale(Ogre::Math::PI);
  auto Camera = w->getCamera();
  Camera->setPosition(0, 0, 300.5f);
  Camera->setAutoAspectRatio(true);

  //  // Setup 2nd scene (2nd widget)
  //  auto scene2 = qtOgre.getScene(1);
  //  auto knotNode2 = scene2->getRootSceneNode()->createChildSceneNode();
  //  auto knotEntity2 = scene2->createEntity("knot.mesh");
  //  knotNode2->attachObject(knotEntity2);
  //  //    knotNode2->setScale(0.01f, 0.01f, 0.01f);
  //  auto sunlight2 = scene2->createLight();
  //  auto sunNode2 = scene2->getRootSceneNode()->createChildSceneNode();
  //  sunNode2->attachObject(sunlight2);
  //  sunlight2->setType(Ogre::Light::LT_DIRECTIONAL);
  //  sunlight2->setDirection(Ogre::Vector3(-1, -1, -0.5f).normalisedCopy());
  //  sunlight2->setPowerScale(Ogre::Math::PI);
  //  auto Camera2 = w2->getCamera();
  //  Camera2->setPosition(0, 0, 300.5f);
  //  //    Camera2->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),
  //  //                                             Ogre::Vector3::UNIT_X));
  //  Camera2->setAutoAspectRatio(true);

  return QApplication::exec();
}
