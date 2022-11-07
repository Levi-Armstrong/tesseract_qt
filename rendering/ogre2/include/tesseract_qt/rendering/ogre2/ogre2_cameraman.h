/****************************************************************************
**
** Copyright (C) 2016 - 2017
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_CAMERAMAN_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_CAMERAMAN_H

#include <Ogre.h>
#include <QtGui>
#include <limits>

namespace tesseract_gui
{
enum CameraMode  // enumerator values for different styles of camera movement
{
  CM_NO,
  CM_BLENDER,
  CM_FLY,   // WIP
  CM_ORBIT  // WIP
};

enum View
{
  VI_TOP,
  VI_LEFT,
  VI_BOTTOM,
  VI_RIGHT,
  VI_FRONT,
  VI_BACK,
  VI_USER
};

enum Direction
{
  DR_FORWARD,
  DR_BACKWARD,
  DR_LEFT,
  DR_RIGHT
};

/****************************************************************************
 * This class manages the ogre camera and transalates Qt events to camera
 * actions
 ***************************************************************************/
class CameraMan
{
public:
  CameraMan(Ogre::Camera* cam)
    : mCamera(0)
    , mTarget(0)
    , mOrbiting(false)
    , mShiftDown(false)
    , mDistFromTarget(0)
    , mCameraNode(0)
    , mCurrentView(VI_USER)
    , mTopSpeed(15)
    , mVelocity(Ogre::Vector3::ZERO)
    , mGoingForward(false)
    , mGoingBack(false)
    , mGoingLeft(false)
    , mGoingRight(false)
    , mGoingUp(false)
    , mGoingDown(false)
    , mMode(CM_NO)
  {
    setCamera(cam);
    setMode(CM_BLENDER);
  }

  virtual ~CameraMan() {}
  Ogre::SceneNode* mCameraNode;

  /*-----------------------------------------------------------------------------
  | Swaps the camera on our camera man for another camera.
  -----------------------------------------------------------------------------*/
  virtual void setCamera(Ogre::Camera* cam)
  {
    mCamera = cam;
    mCameraNode = mCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);
    mCamera->detachFromParent();
    mCameraNode->attachObject(mCamera);
    mCamera->setPosition(Ogre::Vector3(0, 5, 15));
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));
    mCamera->setNearClipDistance(0.1f);
    mCamera->setFarClipDistance(20000.0f);
    mCamera->setAutoAspectRatio(true);
  }

  virtual Ogre::Camera* getCamera() { return mCamera; }

  /*-----------------------------------------------------------------------------
  | Sets the target to revolve around.
  -----------------------------------------------------------------------------*/
  virtual void setTarget(Ogre::SceneNode* target)
  {
    if (target != mTarget)
    {
      mTarget = target;
      if (target)
      {
        setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
      }
      else
      {
        mCamera->setAutoTracking(false);
      }
    }
  }

  /*-----------------------------------------------------------------------------
  | Manually stops the camera when in free-look mode.
  -----------------------------------------------------------------------------*/
  virtual void manualStop()
  {
    if (mMode == CM_FLY)
    {
      mGoingForward = false;
      mGoingBack = false;
      mGoingLeft = false;
      mGoingRight = false;
      mGoingUp = false;
      mGoingDown = false;
      mVelocity = Ogre::Vector3::ZERO;
    }
  }

  virtual Ogre::SceneNode* getTarget() { return mTarget; }

  virtual Ogre::Real getDistanceFromTarget() { return mDistFromTarget; }

  /*-----------------------------------------------------------------------------
  | Sets the spatial offset from the target. Only applies for orbit style.
  -----------------------------------------------------------------------------*/
  virtual void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
  {
    mCamera->setPosition(mTarget->_getDerivedPositionUpdated());
    mCamera->setOrientation(mTarget->_getDerivedOrientationUpdated());
    mCamera->yaw(yaw);
    mCamera->pitch(-pitch);
    mCamera->moveRelative(Ogre::Vector3(0, 0, dist));
  }

  /*-----------------------------------------------------------------------------
| Sets the movement style of our camera man.
-----------------------------------------------------------------------------*/
  virtual void setMode(CameraMode mode)
  {
    if (mMode != CM_BLENDER && mode == CM_BLENDER)
    {
      setTarget(mTarget ? mTarget : mCamera->getSceneManager()->getRootSceneNode());
      setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
    }
    else if (mMode != CM_FLY && mode == CM_FLY)
    {
      mCamera->setAutoTracking(false);
      mCamera->setFixedYawAxis(true);
    }
    mMode = mode;
  }

  virtual void setProjectionType(Ogre::ProjectionType pt)
  {
    if (pt == Ogre::PT_ORTHOGRAPHIC)
    {
      /// @todo: Make orthographic projection work properly.
    }
    else if (pt == Ogre::PT_PERSPECTIVE)
    {
      mCamera->setCustomProjectionMatrix(false);
    }
    mCamera->setProjectionType(pt);
  }

  virtual Ogre::ProjectionType getProjectionType() { return mCamera->getProjectionType(); }

  virtual View getView() { return mCurrentView; }

  virtual void setView(View newView)
  {
    switch (newView)
    {
      case VI_TOP:
        mCameraNode->setOrientation(sqrt(0.5), -sqrt(0.5), 0, 0);
        break;
      case VI_BOTTOM:
        mCameraNode->setOrientation(sqrt(0.5), sqrt(0.5), 0, 0);
        break;
      case VI_LEFT:
        mCameraNode->setOrientation(sqrt(0.5), 0, -sqrt(0.5), 0);
        break;
      case VI_RIGHT:
        mCameraNode->setOrientation(sqrt(0.5), 0, sqrt(0.5), 0);
        break;
      case VI_FRONT:
        mCameraNode->setOrientation(1, 0, 0, 0);
        break;
      case VI_BACK:
        setView(VI_FRONT);  // Recursion
        mCameraNode->setOrientation(0, 0, 1, 0);
    }
    mCurrentView = newView;
  }

  virtual void rotatePerspective(Direction dir)
  {
    Ogre::Radian amount = Ogre::Radian(Ogre::Degree(15));
    switch (dir)
    {
      case DR_FORWARD:
        mCameraNode->rotate(Ogre::Vector3(1, 0, 0), -amount);
        break;
      case DR_BACKWARD:
        mCameraNode->rotate(Ogre::Vector3(1, 0, 0), amount);
        break;
      case DR_LEFT:
        mCameraNode->rotate(Ogre::Vector3(0, 1, 0), -amount, Ogre::Node::TS_WORLD);
        break;
      case DR_RIGHT:
        mCameraNode->rotate(Ogre::Vector3(0, 1, 0), amount, Ogre::Node::TS_WORLD);
        break;
    }
  }

  virtual void numpadViewSwitch(const QKeyEvent* evt)
  {
    bool ctrl = evt->modifiers().testFlag(Qt::ControlModifier);
    bool numpad = evt->modifiers().testFlag(Qt::KeypadModifier);
    if (numpad)
    {
      switch (evt->key())
      {
        case Qt::Key_1:
          setView(ctrl ? VI_BACK : VI_FRONT);
          break;
        case Qt::Key_2:
          rotatePerspective(DR_BACKWARD);
          break;
        case Qt::Key_3:
          setView(ctrl ? VI_LEFT : VI_RIGHT);
          break;
        case Qt::Key_4:
          rotatePerspective(DR_LEFT);
          break;
        case Qt::Key_5:
          setProjectionType((mCamera->getProjectionType() == Ogre::PT_PERSPECTIVE) ? Ogre::PT_ORTHOGRAPHIC :
                                                                                     Ogre::PT_PERSPECTIVE);
          break;
        case Qt::Key_6:
          rotatePerspective(DR_RIGHT);
          break;
        case Qt::Key_7:
          setView(ctrl ? VI_BOTTOM : VI_TOP);
          break;
        case Qt::Key_8:
          rotatePerspective(DR_FORWARD);
          break;
      }
    }
  }

  virtual CameraMode getMode() { return mMode; }

  /*-----------------------------------------------------------------------------
  | Per-frame updates.
  -----------------------------------------------------------------------------*/
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
  {
    if (mMode == CM_FLY)
    {
      // build our acceleration vector based on keyboard input composite
      Ogre::Vector3 accel = Ogre::Vector3::ZERO;
      if (mGoingForward)
        accel += mCamera->getDirection();
      if (mGoingBack)
        accel -= mCamera->getDirection();
      if (mGoingRight)
        accel += mCamera->getRight();
      if (mGoingLeft)
        accel -= mCamera->getRight();
      if (mGoingUp)
        accel += mCamera->getUp();
      if (mGoingDown)
        accel -= mCamera->getUp();

      // if accelerating, try to reach top speed in a certain time
      Ogre::Real topSpeed = mShiftDown ? mTopSpeed * 20 : mTopSpeed;
      if (accel.squaredLength() != 0)
      {
        accel.normalise();
        mVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
      }
      // if not accelerating, try to stop in a certain time
      else
        mVelocity -= mVelocity * evt.timeSinceLastFrame * 10;

      Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

      // keep camera velocity below top speed and above epsilon
      if (mVelocity.squaredLength() > topSpeed * topSpeed)
      {
        mVelocity.normalise();
        mVelocity *= topSpeed;
      }
      else if (mVelocity.squaredLength() < tooSmall * tooSmall)
        mVelocity = Ogre::Vector3::ZERO;

      if (mVelocity != Ogre::Vector3::ZERO)
        mCamera->move(mVelocity * evt.timeSinceLastFrame);
    }
    return true;
  }

  /*-----------------------------------------------------------------------------
  | Processes key presses for free-look style movement.
  -----------------------------------------------------------------------------*/
  virtual void injectKeyDown(const QKeyEvent* evt)
  {
    if (mMode == CM_FLY)
    {
      if (evt->key() == Qt::Key_W)
        mGoingForward = true;
      else if (evt->key() == Qt::Key_A)
        mGoingLeft = true;
      else if (evt->key() == Qt::Key_S)
        mGoingBack = true;
      else if (evt->key() == Qt::Key_D)
        mGoingRight = true;
    }
    if (evt->key() == Qt::Key_Shift)
      mShiftDown = true;
    if (mMode == CM_BLENDER)
      numpadViewSwitch(evt);
  }

  /*-----------------------------------------------------------------------------
  | Processes key releases for free-look style movement.
  -----------------------------------------------------------------------------*/
  virtual void injectKeyUp(const QKeyEvent* evt)
  {
    if (evt->key() == Qt::Key_W)
      mGoingForward = false;
    else if (evt->key() == Qt::Key_A)
      mGoingLeft = false;
    else if (evt->key() == Qt::Key_S)
      mGoingBack = false;
    else if (evt->key() == Qt::Key_D)
      mGoingRight = false;
    if (evt->key() == Qt::Key_Shift)
      mShiftDown = false;
  }

  /*-----------------------------------------------------------------------------
  | Processes mouse movement differently for each style.
  -----------------------------------------------------------------------------*/
  virtual void injectMouseMove(Ogre::Vector2 mousePos)
  {
    if (mMode == CM_FLY)
    {
      mCamera->yaw(Ogre::Degree(-mousePos.x * 0.15f));
      mCamera->pitch(Ogre::Degree(-mousePos.y * 0.15f));
    }
    if (mMode == CM_BLENDER || mMode == CM_ORBIT)
    {
      if (mOrbiting && !mShiftDown)
      {
        rotate(mousePos.x, mousePos.y);
        if (mCurrentView != VI_USER)
          mCurrentView = VI_USER;
      }
      else if ((mOrbiting && mShiftDown) && mMode == CM_BLENDER)
      {
        pan(mousePos.x, mousePos.y);
      }
    }
  }

  virtual void injectMouseWheel(const QWheelEvent* evt)
  {
    mMouseWheelDelta = evt->delta();

    mDistFromTarget = (mCamera->getPosition() - mTarget->_getDerivedPositionUpdated()).length();
    mCamera->moveRelative(Ogre::Vector3(0, 0, -mMouseWheelDelta * 0.0008f * mDistFromTarget));
  }

  virtual void injectMouseDown(const QMouseEvent* evt)
  {
    if (mMode == CM_BLENDER || mMode == CM_ORBIT)
    {
      if (evt->button() == Qt::MiddleButton || evt->button() == Qt::LeftButton)
      {
        mOrbiting = true;
      }
    }
  }

  /*-----------------------------------------------------------------------------
  | Processes mouse releases. Only applies for orbit style.
  | Left button is for orbiting, and right button is for zooming.
  -----------------------------------------------------------------------------*/
  virtual void injectMouseUp(const QMouseEvent* evt)
  {
    if (mMode == CM_BLENDER || mMode == CM_ORBIT)
    {
      if (evt->button() == Qt::MiddleButton || evt->button() == Qt::LeftButton)
      {
        mOrbiting = false;
      }
    }
  }

  virtual void rotate(int x, int y)
  {
    mCameraNode->yaw(Ogre::Degree(-x * 0.4f), Ogre::Node::TS_PARENT);
    mCameraNode->pitch(Ogre::Degree(-y * 0.4f));
  }

  // A non-mouse version of rotate
  virtual void rotate(const Ogre::Vector3& yawPitchRoll)
  {
    mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);
    mCameraNode->yaw(Ogre::Radian(Ogre::Degree(yawPitchRoll.x)), Ogre::Node::TS_PARENT);
    mCameraNode->pitch(Ogre::Radian(Ogre::Degree(yawPitchRoll.y)), Ogre::Node::TS_PARENT);
    mCameraNode->roll(Ogre::Radian(Ogre::Degree(yawPitchRoll.z)), Ogre::Node::TS_PARENT);
  }

  virtual void pan(int x, int y)
  {
    Ogre::Vector3 transVector(-x, y, 0);
    if (mTarget)
    {
      mDistFromTarget = (mCamera->getPosition() - mTarget->_getDerivedPositionUpdated()).length();
      if (mTarget->getAttachedObject(0))
      {
        double radius = mTarget->getAttachedObject(0)->getWorldRadius();
        if (std::isfinite(radius))
          transVector *= radius * (mDistFromTarget / 10000.0f);
      }
    }
    mCameraNode->translate(transVector, Ogre::Node::TS_LOCAL);
  }

  // A non-mouse version of pan
  virtual void pan(const Ogre::Vector3& translate)
  {
    Ogre::Vector3 transVector = translate;
    if (mTarget)
    {
      mDistFromTarget = (mCamera->getPosition() - mTarget->_getDerivedPositionUpdated()).length();
      if (mTarget->getAttachedObject(0))
      {
        double radius = mTarget->getAttachedObject(0)->getWorldRadius();
        if (std::isfinite(radius))
          transVector *= radius * (mDistFromTarget / 10000.0f);
      }
    }
    mCameraNode->setPosition(Ogre::Vector3::ZERO);
    mCameraNode->translate(transVector, Ogre::Node::TS_LOCAL);
  }

  /*-----------------------------------------------------------------------------
  | Set the cameramode to its original position/orientation
  -----------------------------------------------------------------------------*/
  void resetCameraNode(void)
  {
    mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);
    mCameraNode->setPosition(0.0, 0.0, 0.0);
    mTarget->_getDerivedPositionUpdated();
    mTarget->_getDerivedOrientationUpdated();
  }

  /*-----------------------------------------------------------------------------
  | Return orientation
  -----------------------------------------------------------------------------*/
  const Ogre::Quaternion getOrientationCameraSceneNode(void) { return mCameraNode->getOrientation(); }

  /*-----------------------------------------------------------------------------
  | Return position
  -----------------------------------------------------------------------------*/
  const Ogre::Vector3 getPositionCameraSceneNode(void) { return mCameraNode->getPosition(); }

protected:
  Ogre::Camera* mCamera;
  Ogre::SceneNode* mTarget;

  bool mOrbiting;
  bool mShiftDown;
  bool mGoingForward;
  bool mGoingBack;
  bool mGoingLeft;
  bool mGoingRight;
  bool mGoingUp;
  bool mGoingDown;
  Ogre::Vector3 mHelperV3;

  Ogre::Real mDistFromTarget;
  Ogre::Real mTopSpeed;
  Ogre::Vector3 mVelocity;
  int mMouseWheelDelta;

  View mCurrentView;
  CameraMode mMode;
};
}  // namespace tesseract_gui
#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_CAMERAMAN_H
