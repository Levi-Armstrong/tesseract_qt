#include <tesseract_qt/rendering/ogre2/ogre2_viewport.h>

namespace tesseract_gui
{
size_t Ogre2Viewport::window_number_{ 0 };

Ogre2Viewport::Ogre2Viewport(size_t smgr_index, QWidget* parent)
  : QWidget(parent), smgr_index_{ smgr_index }, window_{ nullptr }
{
  // We need QtOgre21 to exist
  if (!Ogre2Manager::instance())
    throw std::runtime_error("Cannot create QOgreViewport widgets if QtOgre21 is not initialized!");

  // Permit Ogre to draw manually on that window
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_PaintOnScreen);

  // Set the title of that window
  setWindowTitle(QString("Window") + QString(std::to_string(window_number_).c_str()));

  // Required to enable keybord focus
  setFocusPolicy(Qt::StrongFocus);

  // Confirugation for Ogre
  Ogre::NameValuePairList misc;
  misc["FSAA"] = std::to_string(Ogre2Manager::instance()->getAALevel());
  misc["vsync"] = "false";
  misc["externalWindowHandle"] = std::to_string(winId());
  misc["stereoMode"] = "Frame Sequential";

#if defined(__APPLE__)
  // Set the macAPI for Ogre based on the Qt implementation
  misc["macAPI"] = "cocoa";
  misc["macAPICocoaUseNSView"] = "true";
#endif

  // Hide window if dimensions are less than or equal to one.
  misc["border"] = "none";

  // Ogre 2 PBS expects gamma correction
  misc["gamma"] = "Yes";

  // This permit to handle some weirdness on window creations
  Ogre2Manager::instance()->willCreateWindowHint();

  // When using the GL3+ renderer, by default the RenderWindow creation create a GL Context.
  // Ogre expect to use only one GL Context, so subsequent window need to use the context of the 1st one.
  if (Ogre2Manager::instance()->getGraphicsAPI() == GraphicsAPI::OPENGL)
  {
    // Get the context. This pointer is nullptr if there's no context created yet
    if (auto context = Ogre2Manager::instance()->getContext())
      // At this point, everything will go bad if this window don't use the same GL context
      misc["externalGLContext"] = std::to_string(size_t(context));
  }

  // Create the RenderWindow
  window_ = Ogre::Root::getSingleton().createRenderWindow(
      std::string("Window") + std::to_string(window_number_++), width(), height(), false, &misc);

  // We have to do this callback. This initialize the scene and the compositor for the rendering, and return a bunch of
  // pointers in a tuple
  auto result = Ogre2Manager::instance()->WidgetCreatedCallback(window_, smgr_index_);

  // Extract the "bunch of usefull pointers" from the tuple
  scene_manager_ = std::get<0>(result);
  camera_ = std::get<1>(result);
  workspace_ = std::get<2>(result);
  workspace_def_name_ = std::get<3>(result);

  // Configure the camera to a state where it will be able to draw something...
  camera_->setNearClipDistance(0.1f);
  camera_->setFarClipDistance(500);
  camera_->setPosition(0, 0, 0);
  camera_->lookAt(0, 0, 0);
  // Camera->setFOVy(Ogre::Degree(45));
  camera_->setAutoAspectRatio(true);

  camera_->detachFromParent();
  camera_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
  camera_node_->attachObject(camera_);

  camera_manager_ = std::make_unique<CameraMan>(camera_);

  system_initialized_ = true;
}

Ogre2Viewport::~Ogre2Viewport() {}

//****************************************************************************/
Ogre::SceneManager* Ogre2Viewport::getSceneManager() { return scene_manager_; }

//****************************************************************************/
Ogre::Camera* Ogre2Viewport::getCamera() { return camera_; }

//****************************************************************************/
Ogre::SceneNode* Ogre2Viewport::getCameraNode() { return camera_node_; }

//****************************************************************************/
void Ogre2Viewport::resetCamera()
{
  camera_->setPosition(Ogre::Vector3(0, 40, 145));
  camera_->lookAt(Ogre::Vector3(0, 0, 0));
  camera_->getParentSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
  //    mLightAxisNode->setPosition(mCamera->getPosition() + Ogre::Vector3(0, -27, -100));
  //    mLightAxisNode->setOrientation(Ogre::Quaternion::IDENTITY);
  //    mSceneNode->setPosition(0.0, 0.0, 0.0);
  //    mSceneNodeRttHoover->setPosition(0.0, 0.0, 0.0);
  //    mSceneNodeRttPaint->setPosition(0.0, 0.0, 0.0);
  camera_manager_->resetCameraNode();
}

//****************************************************************************/
void Ogre2Viewport::setSkyEnabled(bool enabled)
{
  //  Ogre::MaterialPtr skybox_mat;
  //  if (enabled)
  //  {
  //    // get skybox material
  //    std::string skyboxMatName = "Default/skybox";
  //    Ogre::MaterialManager::getSingleton().createOrRetrieve(skyboxMatName,)
  //    skybox_mat = this->Material(skyboxMatName);
  //    if (!skybox_mat)
  //    {
  //      // ogre2 should be able to find this texture as resource search
  //      // paths are already set up in Ogre2RenderEngine.cc
  //      std::string skyboxEnvMap = "skybox.dds";
  //      skybox_mat = this->CreateMaterial(skyboxMatName);
  //      skyboxMat->SetEnvironmentMap(skyboxEnvMap, nullptr);
  //    }
  //  }

  //  camera_->setMaterial();

  //  skybox_enabled_ = enabled;
}

void Ogre2Viewport::setGridEnabled(bool enabled) {}

//****************************************************************************/
void Ogre2Viewport::setRotation(const Ogre::Vector3& rotation)
{
  // Rotation consists of euler angle rotation
  camera_manager_->rotate(rotation);
  reset_mouse_pos_ = true;  // To prevent that the camera shakes when the mouse is used for rotation
}

//****************************************************************************/
void Ogre2Viewport::setPosition(const Ogre::Vector3& position)
{
  camera_manager_->pan(position);
  reset_mouse_pos_ = true;  // To prevent that the camera shakes when the mouse is moved
}

void Ogre2Viewport::paintEvent(QPaintEvent* event)
{
  // Call the refresh of Ogre when the viewport's window is called
  // qDebug() << "paint event";
  Ogre::Root::getSingleton().renderOneFrame();
  window_->windowMovedOrResized();

  event->accept();
}

// Tell Ogre the window has been resized and redraw
void Ogre2Viewport::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  if (event->isAccepted())
  {
    window_->requestResolution(width(), height());
    window_->windowMovedOrResized();

    this->update();
  }
}

// Same as above, but for movement.
void Ogre2Viewport::moveEvent(QMoveEvent* event)
{
  QWidget::moveEvent(event);
  if (event->isAccepted())
  {
    window_->windowMovedOrResized();
    this->update();
  }
}

/* How we handle keyboard and mouse events.*/
void Ogre2Viewport::keyPressEvent(QKeyEvent* event)
{
  if (system_initialized_)
    camera_manager_->injectKeyDown(event);
}

void Ogre2Viewport::keyReleaseEvent(QKeyEvent* event)
{
  if (system_initialized_)
    camera_manager_->injectKeyUp(event);
}

void Ogre2Viewport::mouseMoveEvent(QMouseEvent* event)
{
  if (system_initialized_)
  {
    Ogre::Vector2 oldPos = absolute_;
    absolute_ = Ogre::Vector2(event->pos().x(), event->pos().y());
    if (mouse_down_)
    {
      if (reset_mouse_pos_)
      {
        // Prevent that mRelative becomes to large, so rotation/position isn't smooth
        relative_ = Ogre::Vector2::ZERO;
        reset_mouse_pos_ = false;
      }
      else
      {
        relative_ = absolute_ - oldPos;
      }

      //          if (mRotateCameraMode)
      //          {
      camera_manager_->injectMouseMove(relative_);
      //                    mRenderwindowDockWidget->setCameraOrientationChanged
      //                    (mCameraManager->getOrientationCameraSceneNode());
      //                    mRenderwindowDockWidget->setCameraPositionChanged
      //                    (mCameraManager->getPositionCameraSceneNode());
      //          }
      //          else
      //              rotateLight(mRelative);
    }

    //      // Determine over which subItem the mouse hoovers
    //      if (mHoover)
    //          highlightSubItem(mAbsolute);
    this->update();
  }

  event->accept();
}

void Ogre2Viewport::wheelEvent(QWheelEvent* event)
{
  if (system_initialized_)
  {
    camera_manager_->injectMouseWheel(event);
    this->update();
    //      mLightAxisNode->setPosition(Camera->getPosition() + Ogre::Vector3(0, -27, -100));
  }
}

void Ogre2Viewport::mousePressEvent(QMouseEvent* event)
{
  if (system_initialized_)
  {
    camera_manager_->injectMouseDown(event);

    if (event->button() == Qt::LeftButton)
    {
      mouse_down_ = true;
    }
    else if (event->button() == Qt::RightButton)
    {
      // Forward it to the render window dockwidget
      //                mRenderwindowDockWidget->mousePressEventPublic(e);
    }
  }
}

void Ogre2Viewport::mouseReleaseEvent(QMouseEvent* event)
{
  if (system_initialized_)
  {
    camera_manager_->injectMouseUp(event);
    reset_mouse_pos_ = true;
    if (event->button() == Qt::LeftButton)
      mouse_down_ = false;
  }
  event->accept();

  //  if(Camera)
  //    Camera->injectMouseUp(*e);

  //  QPoint pos = e->pos();
  //  Ogre::Ray mouseRay = Camera->getCameraToViewportRay(
  //    (Ogre::Real)pos.x() / m_ogreWindow->getWidth(),
  //    (Ogre::Real)pos.y() / m_ogreWindow->getHeight());
  //  Ogre::RaySceneQuery* pSceneQuery = SceneManager->createRayQuery(mouseRay);
  //  pSceneQuery->setSortByDistance(true);
  //  Ogre::RaySceneQueryResult vResult = pSceneQuery->execute();
  //  for (size_t ui = 0; ui < vResult.size(); ui++)
  //  {
  //    if (vResult[ui].movable)
  //    {
  //      if (vResult[ui].movable->getMovableType().compare("Entity") == 0)
  //      {
  ////        emit entitySelected((Ogre::Entity*)vResult[ui].movable);
  //      }
  //    }
  //  }
  //  SceneManager->destroyQuery(pSceneQuery);
}

}  // namespace tesseract_gui
