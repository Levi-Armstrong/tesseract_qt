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

#include <QFile>
#include <tesseract_qt/rendering/ogre2/ogre2_widget.h>
#include <tesseract_qt/rendering/ogre2/ogre2_renderman.h>
#include <tesseract_qt/rendering/ogre2/ogre2_constants.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/Pass/OgreCompositorPassDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClearDef.h>
#include <OgreRenderSystem.h>
#include <OgreStagingTexture.h>  //<OgreHardwarePixelBuffer.h>
#include <OgreCommon.h>
#include <OgreTextureGpuManager.h>  //<OgreTextureManager.h>
#include <OgreTimer.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlit.h>
#include <OgreMesh2.h>
#include <OgreSubMesh2.h>
#include <OgreImage2.h>
#include <OgreMeshManager2.h>
#include <OgreStringConverter.h>
//#include "renderwindow_dockwidget.h"

namespace tesseract_gui
{
//****************************************************************************/
QOgreWidget::QOgreWidget(QWidget* parent)
  : QWidget(parent)
  , mRoot(0)
  , mWorkspace(0)
  , mWorkspaceRttHoover(0)
  , mWorkspaceRttPaint(0)
  , mWorkspaceRttSkyBox(0)
  , mRttHoover(0)
  , mRttPaint(0)
  , mOgreWindow(0)
  , mCamera(0)
  , mCameraManager(0)
  , mTimeSinceLastFrame(0.0f)
  , mItem(0)
  , mItemRttHoover(0)
  , mItemRttPaint(0)
  , mLightAxisItem(0)
  , mLight(0)
  , mSceneNode(0)
  , mSceneNodeRttHoover(0)
  , mSceneNodeRttPaint(0)
  , mLightNode(0)
  , mLightAxisNode(0)
  , mSceneCreated(false)
  , mSystemInitialized(false)
  , mRotateCameraMode(true)
  , mShiftDown(false)
  , mMouseDown(false)
  , mLatestSubItemIndexHighlighted(-1)
  , mLatestSubItemDatablock(0)
  , mHoover(false)
  , mPaintMode(false)
  , mOffsetTextureMode(false)
  ,
  //        mPaintLayers(0),
  //        mLatestPaintResult(1),
  mUnlitTextureTypeOffsetTexture(0)
  , mResetMousePos(true)
{
  mPbsDatablockBlockOffsetTexture = "";
  mUnlitDatablockBlockOffsetTexture = "";
  mPbsTextureTypeOffsetTexture = Ogre::PBSM_DIFFUSE;
  mRenderTextureNameHoover = "RenderTargetHlmsEditorTextureHoover";
  mRenderTextureNamePaint = "RenderTargetHlmsEditorTexturePaint";
  setMinimumSize(100, 100);
  mCurrentDatablockId = "";
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_PaintOnScreen);
  mSize = QSize(100, 100);
  resize(mSize);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  mBackground = Ogre::ColourValue(0.02f, 0.02f, 0.02f);
  mAbsolute = Ogre::Vector2::ZERO;
  mRelative = Ogre::Vector2::ZERO;
  mHelpColour = Ogre::ColourValue::Red;
  mSnapshotDatablocks.clear();
  helperIndicesAndNames.clear();

  // Fill the colourmap

  // Index 0-9
  mColourMap[0] = QVector3D(0.1f, 0.0f, 0.0f);
  mColourMap[1] = QVector3D(0.2f, 0.0f, 0.0f);
  mColourMap[2] = QVector3D(0.3f, 0.0f, 0.0f);
  mColourMap[3] = QVector3D(0.4f, 0.0f, 0.0f);
  mColourMap[4] = QVector3D(0.5f, 0.0f, 0.0f);
  mColourMap[5] = QVector3D(0.6f, 0.0f, 0.0f);
  mColourMap[6] = QVector3D(0.7f, 0.0f, 0.0f);
  mColourMap[7] = QVector3D(0.8f, 0.0f, 0.0f);
  mColourMap[8] = QVector3D(0.9f, 0.0f, 0.0f);
  mColourMap[9] = QVector3D(1.0f, 0.0f, 0.0f);

  // Index 10-19
  mColourMap[10] = QVector3D(0.0f, 0.1f, 0.0f);
  mColourMap[11] = QVector3D(0.0f, 0.2f, 0.0f);
  mColourMap[12] = QVector3D(0.0f, 0.3f, 0.0f);
  mColourMap[13] = QVector3D(0.0f, 0.4f, 0.0f);
  mColourMap[14] = QVector3D(0.0f, 0.5f, 0.0f);
  mColourMap[15] = QVector3D(0.0f, 0.6f, 0.0f);
  mColourMap[16] = QVector3D(0.0f, 0.7f, 0.0f);
  mColourMap[17] = QVector3D(0.0f, 0.8f, 0.0f);
  mColourMap[18] = QVector3D(0.0f, 0.9f, 0.0f);
  mColourMap[19] = QVector3D(0.0f, 1.0f, 0.0f);

  // Index 20-29
  mColourMap[20] = QVector3D(0.0f, 0.0f, 0.1f);
  mColourMap[21] = QVector3D(0.0f, 0.0f, 0.2f);
  mColourMap[22] = QVector3D(0.0f, 0.0f, 0.3f);
  mColourMap[23] = QVector3D(0.0f, 0.0f, 0.4f);
  mColourMap[24] = QVector3D(0.0f, 0.0f, 0.5f);
  mColourMap[25] = QVector3D(0.0f, 0.0f, 0.6f);
  mColourMap[26] = QVector3D(0.0f, 0.0f, 0.7f);
  mColourMap[27] = QVector3D(0.0f, 0.0f, 0.8f);
  mColourMap[28] = QVector3D(0.0f, 0.0f, 0.9f);
  mColourMap[29] = QVector3D(0.0f, 0.0f, 1.0f);

  // Index 30-39
  mColourMap[30] = QVector3D(0.1f, 0.1f, 0.0f);
  mColourMap[31] = QVector3D(0.2f, 0.2f, 0.0f);
  mColourMap[32] = QVector3D(0.3f, 0.3f, 0.0f);
  mColourMap[33] = QVector3D(0.4f, 0.4f, 0.0f);
  mColourMap[34] = QVector3D(0.5f, 0.5f, 0.0f);
  mColourMap[35] = QVector3D(0.6f, 0.6f, 0.0f);
  mColourMap[36] = QVector3D(0.7f, 0.7f, 0.0f);
  mColourMap[37] = QVector3D(0.8f, 0.8f, 0.0f);
  mColourMap[38] = QVector3D(0.9f, 0.9f, 0.0f);
  mColourMap[39] = QVector3D(1.0f, 1.0f, 0.0f);

  // Index 40-49
  mColourMap[40] = QVector3D(0.1f, 0.0f, 0.1f);
  mColourMap[41] = QVector3D(0.2f, 0.0f, 0.2f);
  mColourMap[42] = QVector3D(0.3f, 0.0f, 0.3f);
  mColourMap[43] = QVector3D(0.4f, 0.0f, 0.4f);
  mColourMap[44] = QVector3D(0.5f, 0.0f, 0.5f);
  mColourMap[45] = QVector3D(0.6f, 0.0f, 0.6f);
  mColourMap[46] = QVector3D(0.7f, 0.0f, 0.7f);
  mColourMap[47] = QVector3D(0.8f, 0.0f, 0.8f);
  mColourMap[48] = QVector3D(0.9f, 0.0f, 0.9f);
  mColourMap[49] = QVector3D(1.0f, 0.0f, 1.0f);

  // Index 50-59
  mColourMap[50] = QVector3D(0.0f, 0.1f, 0.1f);
  mColourMap[51] = QVector3D(0.0f, 0.2f, 0.2f);
  mColourMap[52] = QVector3D(0.0f, 0.3f, 0.3f);
  mColourMap[53] = QVector3D(0.0f, 0.4f, 0.4f);
  mColourMap[54] = QVector3D(0.0f, 0.5f, 0.5f);
  mColourMap[55] = QVector3D(0.0f, 0.6f, 0.6f);
  mColourMap[56] = QVector3D(0.0f, 0.7f, 0.7f);
  mColourMap[57] = QVector3D(0.0f, 0.8f, 0.8f);
  mColourMap[58] = QVector3D(0.0f, 0.9f, 0.9f);
  mColourMap[59] = QVector3D(0.0f, 1.0f, 1.0f);
}

//****************************************************************************/
QOgreWidget::~QOgreWidget()
{
  // Cannot destroy the render texture here, because that would be too late (Ogre root is already deleted)
}

//****************************************************************************/
void QOgreWidget::cleanup(void)
{
  Ogre::CompositorManager2* compositorManager = mRoot->getCompositorManager2();
  compositorManager->removeAllWorkspaceDefinitions();
  mWorkspace = 0;
  mWorkspaceRttHoover = 0;
  mWorkspaceRttPaint = 0;

  mRoot->getRenderSystem()->getTextureGpuManager()->destroyTexture(mCustomRenderTextureHoover);
  mRoot->getRenderSystem()->getTextureGpuManager()->destroyTexture(mCustomRenderTexturePaint);
  mCustomRenderTextureHoover = nullptr;
  mCustomRenderTexturePaint = nullptr;
}

//****************************************************************************/
GL_CONTEXT QOgreWidget::getCurrentGlContext(void)
{
#if defined(Q_OS_WIN)
  return wglGetCurrentContext();  // Windows
#else
  return glXGetCurrentContext();  // Linux
#endif

  return 0;
}

//****************************************************************************/
void QOgreWidget::createRenderWindow(OgreManager* ogreManager)
{
  if (!ogreManager)
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "No Ogre Manager available", "QOgreWidget::createRenderWindow");

  Ogre::Root* root = ogreManager->getOgreRoot();
  if (!root)
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "No Ogre Root available", "QOgreWidget::createRenderWindow");

  // Get rendersystem and assign window handle
  mRoot = root;
  Ogre::NameValuePairList parameters;
  parameters.insert(std::make_pair("gamma", "true"));  // This is needed to have the rtt's correct and to make the
                                                       // materials look good

  // Reuse the glContext if available
  GL_CONTEXT glContext = 0;
  if (ogreManager->isRenderSystemGL())
  {
    parameters["currentGLContext"] = Ogre::String("false");
    glContext = ogreManager->getGlContext();
    if (glContext)
    {
      parameters["externalGLContext"] = Ogre::StringConverter::toString((size_t)(glContext));
      parameters["vsync"] = "No";
    }
  }

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
  Ogre::String windowHandle = Ogre::StringConverter::toString((size_t)(this->winId()));
  parameters["externalWindowHandle"] = windowHandle;
  parameters["parentWindowHandle"] = windowHandle;
#else
  Ogre::String windowHandle = Ogre::StringConverter::toString((unsigned long)(this->winId()));
  parameters["externalWindowHandle"] = windowHandle;
  parameters["parentWindowHandle"] = windowHandle;
#endif

#if defined(Q_OS_MAC)
  parameters["macAPI"] = "cocoa";
  parameters["macAPICocoaUseNSView"] = "true";
#endif

  mOgreWindow = mRoot->createRenderWindow(
      std::to_string(mRoot->getTimer()->getMicroseconds()), this->width(), this->height(), false, &parameters);
  mOgreWindow->setHidden(false);
  //        mOgreWindow->setVisible(true);

  // Determine whether the GL context can be reused
  if (ogreManager->isRenderSystemGL() && !glContext)
  {
    // Store the glContext in the ogre manager
    glContext = getCurrentGlContext();
    ogreManager->setGlContext(glContext);
  }

  // Create scene manager
  const size_t numThreads = std::max<int>(1, Ogre::PlatformInformation::getNumLogicalCores());
  //        Ogre::InstancingThreadedCullingMethod threadedCullingMethod = (numThreads > 1) ?
  //        Ogre::INSTANCING_CULLING_THREADED : Ogre::INSTANCING_CULLING_SINGLETHREAD;
  mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, numThreads);
  mSceneManager->getRenderQueue()->setRenderQueueMode(1, Ogre::RenderQueue::FAST);
  mSceneManager->getRenderQueue()->setRenderQueueMode(2, Ogre::RenderQueue::FAST);
  mSceneManager->getRenderQueue()->setRenderQueueMode(3, Ogre::RenderQueue::FAST);
  mSceneManager->setShadowDirectionalLightExtrusionDistance(500.0f);
  mSceneManager->setShadowFarDistance(500.0f);

  // Create camera
  mCamera = mSceneManager->createCamera("MainCamera");
  mCamera->setAspectRatio(Ogre::Real(mOgreWindow->getWidth()) / Ogre::Real(mOgreWindow->getHeight()));
  mCameraManager = new CameraMan(mCamera);
}

//****************************************************************************/
void QOgreWidget::createScene()
{
  // Create the compositor
  createCompositor();

  // Create the compositor RTT
  createCompositorRenderToTexture();

  // Create the skybox compositor
  createSkyBoxCompositor();

  // Create the node and attach the entity
  mSceneNode = mSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(Ogre::SCENE_DYNAMIC);
  mSceneNode->setPosition(0.0, 0.0, 0.0);
  mSceneNodeRttHoover = mSceneNode->createChildSceneNode(Ogre::SCENE_DYNAMIC);
  mSceneNodeRttHoover->setPosition(0.0, 0.0, 0.0);
  mSceneNodeRttPaint = mSceneNode->createChildSceneNode(Ogre::SCENE_DYNAMIC);
  mSceneNodeRttPaint->setPosition(0.0, 0.0, 0.0);
  mCameraManager->setTarget(mSceneNode);

  // Create an item
  // Ogre::Vector3 scale(20.0f, 20.0f, 20.0f);
  // createItem ("cube.mesh", scale);
  Ogre::Vector3 scale(5.0f, 5.0f, 5.0f);
  createItem("materialball.mesh", scale);

  // Remove the datablock currently set on this item
  // Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  // Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>( hlmsManager->getHlms(Ogre::HLMS_PBS) );

  setDefaultDatablockItem();

  // if (hlmsPbs->getDatablock(DATABLOCK_DEBUG_CUBE))
  // hlmsPbs->destroyDatablock(DATABLOCK_DEBUG_CUBE);

  // Create static light; make it a bit warm
  mSecondLight = mSceneManager->createLight();
  mSecondLightNode = mSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(Ogre::SCENE_DYNAMIC);
  mSecondLightNode->attachObject(mSecondLight);
  mSecondLight->setDiffuseColour(1.0f, 0.85f, 0.7f);
  mSecondLight->setSpecularColour(1.0f, 0.85f, 0.7f);
  mSecondLight->setPowerScale(Ogre::Math::PI);
  mSecondLight->setAttenuation(15000.0f, 0.0f, 0.0f, 0.0f);
  mSecondLight->setType(Ogre::Light::LT_DIRECTIONAL);
  mSecondLightNode->setPosition(100.0f, 500.0f, 300.0f);
  mSecondLight->setDirection(Ogre::Vector3(-1, -5, -3).normalisedCopy());
  mSecondLight->setVisible(true);

  // Create rotating light
  mLight = mSceneManager->createLight();
  mLightNode = mCameraManager->mCameraNode->createChildSceneNode(Ogre::SCENE_DYNAMIC);
  mLightNode->attachObject(mLight);
  mLight->setPowerScale(Ogre::Math::PI);  // Since we don't do HDR, counter the PBS' division by PI
  mLight->setAttenuation(15000.0f, 0.0f, 0.0f, 0.0f);
  mLight->setType(Ogre::Light::LT_DIRECTIONAL);
  mLight->setDiffuseColour(Ogre::ColourValue::White);
  mLight->setSpecularColour(Ogre::ColourValue::White);
  mLight->setDirection(Ogre::Vector3(0, 1, 0));
  mLight->setVisible(true);

  // Light axis node
  mLightAxisNode = mCameraManager->mCameraNode->createChildSceneNode(Ogre::SCENE_DYNAMIC);
  mLightAxisNode->setPosition(mCamera->getPosition() + Ogre::Vector3(0, -27, -100));
  mLightAxisItem = mSceneManager->createItem(
      "axis.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);
  mLightAxisItem->setRenderQueueGroup(3);
  mLightAxisNode->attachObject(mLightAxisItem);
  mLightAxisNode->setScale(Ogre::Vector3(0.12f, 0.12f, 0.12f));
  createLightAxisMaterial();
  mLightAxisItem->setVisible(false);
  createHighlightMaterial();

  // Put some light at the bottom, so the materials are not completely dark
  mSceneManager->setAmbientLight(
      Ogre::ColourValue::White, Ogre::ColourValue::White, Ogre::Vector3(0, 1, 0).normalisedCopy());

  resetCamera();

  // Create the uv mapping colourmap
  createUnlitDatablockRttPaint();

  mSystemInitialized = true;
}

//****************************************************************************/
void QOgreWidget::createLightAxisMaterial(void)
{
  try
  {
    Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
    Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
    Ogre::HlmsDatablock* datablock = hlmsPbs->getDatablock(AXIS_MATERIAL_NAME);

    // Return  if the datablock already exists
    if (datablock)
      return;

    // Create a Pbs datablock
    Ogre::HlmsMacroblock macroblock;
    macroblock.mDepthCheck = false;
    macroblock.mDepthWrite = false;
    Ogre::HlmsPbsDatablock* datablockPbs = static_cast<Ogre::HlmsPbsDatablock*>(hlmsPbs->createDatablock(
        AXIS_MATERIAL_NAME, AXIS_MATERIAL_NAME, macroblock, Ogre::HlmsBlendblock(), Ogre::HlmsParamVec()));
    datablockPbs->setDiffuse(Ogre::Vector3(1, 0, 0));
    mLightAxisItem->setDatablock(AXIS_MATERIAL_NAME);
  }
  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
void QOgreWidget::destroyLightAxisMaterial(void)
{
  try
  {
    mLightAxisItem->setDatablock(DEFAULT_DATABLOCK_NAME);
    Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
    Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
    Ogre::HlmsDatablock* datablock = hlmsPbs->getDatablock(AXIS_MATERIAL_NAME);
    if (datablock && datablock->getLinkedRenderables().size() == 0)
      hlmsPbs->destroyDatablock(AXIS_MATERIAL_NAME);
  }
  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
void QOgreWidget::createHighlightMaterial(void)
{
  // The highlight material is used for mItem
  try
  {
    // Create a Pbs datablock
    Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
    Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
    Ogre::HlmsMacroblock macroblock;
    macroblock.mDepthCheck = true;
    macroblock.mDepthWrite = true;
    Ogre::HlmsPbsDatablock* datablock = static_cast<Ogre::HlmsPbsDatablock*>(hlmsPbs->createDatablock(
        HIGHLIGHT_MATERIAL_NAME, HIGHLIGHT_MATERIAL_NAME, macroblock, Ogre::HlmsBlendblock(), Ogre::HlmsParamVec()));
    datablock->setDiffuse(Ogre::Vector3(0.0f, 1.0f, 0.0f));
    datablock->setSpecular(Ogre::Vector3(0.0f, 1.0f, 0.0f));
  }
  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
void QOgreWidget::destroyHighlightMaterial(void)
{
  resetHighlight();
  try
  {
    Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
    Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
    if (hlmsPbs->getDatablock(HIGHLIGHT_MATERIAL_NAME))
      hlmsPbs->destroyDatablock(HIGHLIGHT_MATERIAL_NAME);
  }
  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
bool QOgreWidget::isSkyBoxVisible(void) { return mWorkspaceRttSkyBox->getEnabled(); }

//****************************************************************************/
void QOgreWidget::setSkyBoxVisible(bool visible) { mWorkspaceRttSkyBox->setEnabled(visible); }

//****************************************************************************/
void QOgreWidget::updateSkyBoxMaterial(const Ogre::String& cubeMapBaseFileName)
{
  Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingletonPtr()->getByName(SKYBOX_MATERIAL_NAME);
  if (materialPtr.isNull())
    return;

  Ogre::Material* material = materialPtr.getPointer();
  Ogre::TextureUnitState* tex = material->getTechnique(0)->getPass(0)->getTextureUnitState(0);
  tex->setCubicTextureName(cubeMapBaseFileName, true);
  tex->setGamma(2.0);
  material->compile();
  setSkyBoxVisible(true);
}

//****************************************************************************/
void QOgreWidget::createItem(const Ogre::String& meshName, const Ogre::Vector3& scale)
{
  try
  {
    Ogre::String datablockNameStr = "";

    // Delete the old item if available
    if (mItem)
    {
      // Set the default material to the item
      datablockNameStr = *(mItem->getSubItem(0)->getDatablock()->getNameStr());
      setDefaultDatablockItem();

      // Delete the item
      mSceneNode->detachAllObjects();
      mSceneManager->destroyItem(mItem);

      // Also delete the mesh, because meshes with the same name (although different meshes) may be loaded
      Ogre::MeshManager* man = Ogre::MeshManager::getSingletonPtr();
      if (man->getResourceByName(meshName))
      {
        man->remove(meshName);
      }
    }

    // Create a new item
    mItem = mSceneManager->createItem(
        meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);
    mSceneNode->attachObject(mItem);
    mSceneNode->setScale(scale);
    mItem->setRenderQueueGroup(2);

    // Delete the old itemRttHoover if available
    if (mItemRttHoover)
    {
      destroyUnlitDatablocksRttHoover();
      mSceneNodeRttHoover->detachAllObjects();
      mSceneManager->destroyItem(mItemRttHoover);
    }

    // Create a new itemRttHoover
    mItemRttHoover = mSceneManager->createItem(
        meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);

    mSceneNodeRttHoover->attachObject(mItemRttHoover);
    mSceneNodeRttHoover->setVisible(false);
    createUnlitDatablocksRttHoover();
    mItemRttHoover->setRenderQueueGroup(2);

    // Delete the old itemRttPaint if available
    if (mItemRttPaint)
    {
      setDefaultDatablockItemRttPaint();  // Don't destroy the datablock, because it will be reused
      mSceneNodeRttPaint->detachAllObjects();
      mSceneManager->destroyItem(mItemRttPaint);
    }

    // Create a new itemRttPaint
    mItemRttPaint = mSceneManager->createItem(
        meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);

    mSceneNodeRttPaint->attachObject(mItemRttPaint);
    setUnlitDatablockRttPaint();  // Don't recreate the datablock if not needed
    mSceneNodeRttPaint->setVisible(false);
    mItemRttPaint->setRenderQueueGroup(1);  // Must be lower than the render queue used for the mItemRttHoover,
                                            // even when the node is not visisble, the result is that the material of
                                            // the mItemRttHoover cannot be seen (obscured by the mItemRttPaint?)

// Put an extra renderOneFrame, because of an exception in Debug (D3D11 device cannot Clear State)
#if _DEBUG || DEBUG
    mRoot->renderOneFrame();
#endif
  }

  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
void QOgreWidget::setDefaultDatablockItem(void)
{
  // If one of the subItems was highlighted, restore it
  resetHighlight();

  // Set the default datablock
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));

  bool success = false;
  try
  {
    mItem->setDatablock(hlmsPbs->getDefaultDatablock()->getName());
    success = true;
  }
  catch (Ogre::Exception e)
  {
  }
  if (!success)
  {
    try
    {
      mItem->setDatablock(DEFAULT_DATABLOCK_NAME);
      success = true;
    }
    catch (Ogre::Exception e)
    {
    }
  }
  if (!success)
  {
    try
    {
      mItem->setDatablock(hlmsUnlit->getDefaultDatablock()->getName());
    }
    catch (Ogre::Exception e)
    {
    }
  }
}

//****************************************************************************/
void QOgreWidget::setDefaultDatablockSubItem(Ogre::SubItem* subItem)
{
  // If one of the subItems was highlighted, restore it
  resetHighlight();

  // Set the default datablock
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));

  bool success = false;
  try
  {
    subItem->setDatablock(hlmsPbs->getDefaultDatablock()->getName());
    success = true;
  }
  catch (Ogre::Exception e)
  {
  }
  if (!success)
  {
    try
    {
      subItem->setDatablock(DEFAULT_DATABLOCK_NAME);
      success = true;
    }
    catch (Ogre::Exception e)
    {
    }
  }
  if (!success)
  {
    try
    {
      subItem->setDatablock(hlmsUnlit->getDefaultDatablock()->getName());
    }
    catch (Ogre::Exception e)
    {
    }
  }
}

//****************************************************************************/
void QOgreWidget::setDefaultDatablockInSubItem(int index)
{
  if (mItem)
  {
    Ogre::SubItem* subItem = mItem->getSubItem(index);
    if (subItem)
      setDefaultDatablockSubItem(subItem);
  }
}

//****************************************************************************/
void QOgreWidget::setDefaultDatablockItemRttHoover(void)
{
  resetHighlight();
  Ogre::HlmsDatablock* itemRttHooverDatablock = mItemRttHoover->getSubItem(0)->getDatablock();
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));

  if (itemRttHooverDatablock != hlmsUnlit->getDefaultDatablock())
    mItemRttHoover->setDatablock(hlmsUnlit->getDefaultDatablock()->getName());
  else if (itemRttHooverDatablock != hlmsPbs->getDefaultDatablock())
    mItemRttHoover->setDatablock(hlmsPbs->getDefaultDatablock()->getName());
  else
    mItemRttHoover->setDatablock(DEFAULT_DATABLOCK_NAME);
}

//****************************************************************************/
void QOgreWidget::createUnlitDatablocksRttHoover(void)
{
  // Iterate through the subItems of mItemRttHoover and assign a colourvalue that corresponds with the subItem index
  // Set an unlit datablock with that colour
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));
  Ogre::HlmsMacroblock macroblock;
  Ogre::HlmsBlendblock blendblock;
  Ogre::ColourValue colour;
  Ogre::String datablockId;

  // Create a new datablock
  size_t numSubItems = mItemRttHoover->getNumSubItems();
  Ogre::SubItem* subItem;
  for (size_t i = 0; i < numSubItems; ++i)
  {
    subItem = mItemRttHoover->getSubItem(i);
    datablockId = Ogre::StringConverter::toString(i);
    Ogre::HlmsUnlitDatablock* datablock = static_cast<Ogre::HlmsUnlitDatablock*>(
        hlmsUnlit->createDatablock(datablockId, datablockId, macroblock, blendblock, Ogre::HlmsParamVec()));
    colour = calculateIndexToColour(i);
    datablock->setUseColour(true);
    datablock->setColour(colour);
    subItem->setDatablock(datablockId);
  }
}

//****************************************************************************/
void QOgreWidget::destroyUnlitDatablocksRttHoover(void)
{
  // Detach all datablocks from mItemRttHoover
  setDefaultDatablockItemRttHoover();

  // Destroy all unlit materials; assume they are not attached anymore
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));
  size_t numSubItems = mItemRttHoover->getNumSubItems();
  for (size_t i = 0; i < numSubItems; ++i)
  {
    hlmsUnlit->destroyDatablock(Ogre::StringConverter::toString(i));
  }
}

//****************************************************************************/
void QOgreWidget::setDefaultDatablockItemRttPaint(void)
{
  Ogre::HlmsDatablock* itemRttPaintDatablock = mItemRttPaint->getSubItem(0)->getDatablock();
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));

  if (itemRttPaintDatablock != hlmsUnlit->getDefaultDatablock())
    mItemRttPaint->setDatablock(hlmsUnlit->getDefaultDatablock()->getName());
  else if (itemRttPaintDatablock != hlmsPbs->getDefaultDatablock())
    mItemRttPaint->setDatablock(hlmsPbs->getDefaultDatablock()->getName());
  else
    mItemRttPaint->setDatablock(DEFAULT_DATABLOCK_NAME);
}

//****************************************************************************/
void QOgreWidget::createUnlitDatablockRttPaint(void)
{
  Ogre::String uvMappingTextureFileName = OGRE3_PATH + UV_MAPPING_TEXTURE_NAME;
#ifdef CREATE_UV_MAPPING_TEXTURE
  /* Create the custom uv mapping texture if CREATE_UV_TEXTURE exists. This only to generate the texture once.
   * After that, we don't need this anymore, but the code still remains to recreate the texture if needed.
   * Creation of the texture could also be done outside the application, but for convenience it is
   * part of the HLMS editor.
   *
   * Note, that a gamma value is applied to the texture, which increases smooth painting. Not sure whether
   * this gives the same effect on other computers.
   */

  Ogre::Image uvMap;
  Ogre::uint32 width = 1024;
  Ogre::uint32 height = 1024;
  float u = 0.0f;
  float v = 0.0f;
  size_t formatSize = Ogre::PixelUtil::getNumElemBytes(Ogre::PF_R8G8B8A8);
  uchar* data = new uchar[width * height * formatSize];
  uvMap.loadDynamicImage(data, width, height, Ogre::PF_A8R8G8B8);
  Ogre::PixelBox pixelbox = uvMap.getPixelBox(0, 0);
  Ogre::ColourValue col;
  float gamma = 2.2f;
  float gammaCorrection = 1.0f / gamma;
  col.g = 0.0f;
  for (size_t y = 0; y < height; y++)
  {
    v = (float)y / (float)height;
    col.b = std::pow(v, gammaCorrection);
    for (size_t x = 0; x < width; x++)
    {
      u = (float)x / (float)width;
      col.r = std::pow(u, gammaCorrection);
      pixelbox.setColourAt(col, x, y, 0);
    }
  }
  uvMap.save(uvMappingTextureFileName);
  delete[] data;

#endif

  // Create the datablock with a custom UV texture from file
  // If the file already exist, it is loaded at the start of the application because
  // it must be in a directory defined in file resources.cfg
  try
  {
    // Create a HLMS Unlit datablock
    Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
    Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));
    Ogre::HlmsMacroblock macroblock;
    macroblock.mDepthCheck = true;
    macroblock.mDepthWrite = true;
    Ogre::HlmsUnlitDatablock* datablock = static_cast<Ogre::HlmsUnlitDatablock*>(hlmsUnlit->createDatablock(
        UV_MAPPING_MATERIAL_NAME, UV_MAPPING_MATERIAL_NAME, macroblock, Ogre::HlmsBlendblock(), Ogre::HlmsParamVec()));

    // Attach samplerblock
    Ogre::TextureGpuManager* hlmsTextureManager = mRoot->getRenderSystem()->getTextureGpuManager();
    Ogre::TextureGpu* texLocation =
        hlmsTextureManager->createOrRetrieveTexture(UV_MAPPING_TEXTURE_NAME,
                                                    Ogre::GpuPageOutStrategy::SaveToSystemRam,
                                                    Ogre::TextureFlags::RenderToTexture,
                                                    Ogre::TextureTypes::Type2D);
    //            Ogre::HlmsSamplerblock samplerblock;
    datablock->setTexture(0, texLocation);
    //            datablock->setSamplerblock(0, samplerblock);
    setUnlitDatablockRttPaint();
  }
  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
const Ogre::Vector2& QOgreWidget::calculateColourToUv(const Ogre::ColourValue& col)
{
  helperVector2.x = col.r;
  helperVector2.y = col.b;
  return helperVector2;
}

//****************************************************************************/
void QOgreWidget::setUnlitDatablockRttPaint(void)
{
  if (mItemRttPaint)
  {
    size_t numSubItems = mItemRttPaint->getNumSubItems();
    Ogre::SubItem* subItem;
    for (size_t i = 0; i < numSubItems; ++i)
    {
      subItem = mItemRttPaint->getSubItem(i);
      subItem->setDatablock(UV_MAPPING_MATERIAL_NAME);
    }
  }
}

//****************************************************************************/
void QOgreWidget::destroyUnlitDatablockRttPaint(void)
{
  // Detach datablock from mItemRttPaint
  setDefaultDatablockItemRttPaint();

  try
  {
    Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
    Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));
    if (hlmsUnlit->getDatablock(UV_MAPPING_MATERIAL_NAME))
      hlmsUnlit->destroyDatablock(UV_MAPPING_MATERIAL_NAME);
  }
  catch (Ogre::Exception e)
  {
  }
}

//****************************************************************************/
const Ogre::ColourValue& QOgreWidget::calculateIndexToColour(int index)
{
  QVector3D v = mColourMap[index];
  mHelpColour.r = v.x();
  mHelpColour.g = v.y();
  mHelpColour.b = v.z();
  return mHelpColour;
}

//****************************************************************************/
int QOgreWidget::calculateColourToIndex(const Ogre::ColourValue& colourValue)
{
  QVector3D v;
  int index = -1;
  QMap<int, QVector3D>::iterator it = mColourMap.begin();
  QMap<int, QVector3D>::iterator itEnd = mColourMap.end();
  while (it != itEnd)
  {
    v = it.value();
    index = it.key();
    if (qAbs(v.x() - colourValue.r) < 0.05f && qAbs(v.y() - colourValue.g) < 0.05f &&
        qAbs(v.z() - colourValue.b) < 0.05f)
      return index;

    ++it;
  }

  return -1;
}

//****************************************************************************/
void QOgreWidget::setBackgroundColour(const Ogre::ColourValue colour)
{
  mBackground = colour;
  Ogre::CompositorManager2* compositorManager = mRoot->getCompositorManager2();
  Ogre::CompositorManager2::CompositorNodeDefMap mNodeDefinitions = compositorManager->getNodeDefinitions();
  Ogre::CompositorManager2::CompositorNodeDefMap::const_iterator iter;
  Ogre::CompositorManager2::CompositorNodeDefMap::const_iterator iterStart = mNodeDefinitions.begin();
  Ogre::CompositorManager2::CompositorNodeDefMap::const_iterator iterEnd = mNodeDefinitions.end();
  Ogre::CompositorNodeDef* nodeDef;
  Ogre::CompositorTargetDef* targetDef;
  Ogre::CompositorPassDef* passDef;
  Ogre::CompositorPassClearDef* clearDef;
  for (iter = iterStart; iter != iterEnd; ++iter)
  {
    nodeDef = iter->second;
    if (nodeDef)
    {
      if (nodeDef->getNumTargetPasses() > 0)
      {
        targetDef = nodeDef->getTargetPass(0);
        Ogre::CompositorPassDefVec passDefs = targetDef->getCompositorPasses();
        Ogre::CompositorPassDefVec::const_iterator iterPass;
        Ogre::CompositorPassDefVec::const_iterator iterPassStart = passDefs.begin();
        Ogre::CompositorPassDefVec::const_iterator iterPassEnd = passDefs.end();
        for (iterPass = iterPassStart; iterPass != iterPassEnd; ++iterPass)
        {
          passDef = *iterPass;
          if (Ogre::PASS_CLEAR == passDef->getType())
          {
            clearDef = static_cast<Ogre::CompositorPassClearDef*>(passDef);
            //                            clearDef->mColourValue = colour; TODO Levi Figure out how to fix
          }
        }
      }
    }
  }
}

//****************************************************************************/
void QOgreWidget::createCompositor()
{
  Ogre::CompositorManager2* compositorManager = mRoot->getCompositorManager2();
  const Ogre::String workspaceName = std::to_string(mRoot->getTimer()->getMicroseconds());
  const Ogre::IdString workspaceNameHash = workspaceName;
  compositorManager->createBasicWorkspaceDef(workspaceName, mBackground);
  mWorkspace = compositorManager->addWorkspace(
      mSceneManager, mOgreWindow->getTexture(), mCamera, workspaceNameHash, true);  // Auto-update, because manual
                                                                                    // update gives issues
}

//****************************************************************************/
void QOgreWidget::createCompositorRenderToTexture(void)
{
  // Create a render-texture to determine on which subitem the mouse pointer is pointing at
  Ogre::TextureGpuManager* textureMgr = mRoot->getRenderSystem()->getTextureGpuManager();
  mCustomRenderTextureHoover =
      textureMgr->createOrRetrieveTexture(mRenderTextureNameHoover,
                                          Ogre::GpuPageOutStrategy::SaveToSystemRam,
                                          Ogre::TextureFlags::RenderToTexture,
                                          Ogre::TextureTypes::Type2D,
                                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  mCustomRenderTextureHoover->setResolution(RTT_HOOVER_SIZE_X, RTT_HOOVER_SIZE_Y);
  mCustomRenderTextureHoover->setNumMipmaps(1u);
  mCustomRenderTextureHoover->setPixelFormat(Ogre::PFG_R8G8_B8G8_UNORM);
  mCustomRenderTextureHoover->scheduleTransitionTo(Ogre::GpuResidency::Resident);
  //        mCustomRenderTextureHoover =
  //        mRoot->getRenderSystem()->getTextureGpuManager()->createManual(mRenderTextureNameHoover,
  //                                                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
  //                                                                                                    Ogre::TEX_TYPE_2D,
  //                                                                                                    RTT_HOOVER_SIZE_X,
  //                                                                                                    RTT_HOOVER_SIZE_Y,
  //                                                                                                    1,
  //                                                                                                    Ogre::PF_R8G8B8A8,
  //                                                                                                    Ogre::TU_RENDERTARGET);
  //        mCustomRenderTextureHoover->load();
  mRttHoover = mCustomRenderTextureHoover;
  Ogre::CompositorManager2* compositorManager = mRoot->getCompositorManager2();
  const Ogre::String workspaceNameHoover = std::to_string(mRoot->getTimer()->getMicroseconds());
  const Ogre::IdString workspaceNameHooverHash = workspaceNameHoover;
  compositorManager->createBasicWorkspaceDef(workspaceNameHoover, Ogre::ColourValue::Black);
  mWorkspaceRttHoover =
      compositorManager->addWorkspace(mSceneManager, mRttHoover, mCamera, workspaceNameHooverHash, false);

  // Create a render-texture for painting
  mCustomRenderTexturePaint =
      textureMgr->createOrRetrieveTexture(mRenderTextureNamePaint,
                                          Ogre::GpuPageOutStrategy::SaveToSystemRam,
                                          Ogre::TextureFlags::RenderToTexture,
                                          Ogre::TextureTypes::Type2D,
                                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  mCustomRenderTexturePaint->setResolution(RTT_PAINT_SIZE_X, RTT_PAINT_SIZE_Y);
  mCustomRenderTexturePaint->setNumMipmaps(1u);
  mCustomRenderTexturePaint->setPixelFormat(Ogre::PFG_R8G8_B8G8_UNORM);
  mCustomRenderTexturePaint->scheduleTransitionTo(Ogre::GpuResidency::Resident);
  //        mCustomRenderTexturePaint =
  //        mRoot->getRenderSystem()->getTextureGpuManager()->createManual(mRenderTextureNamePaint,
  //                                                                                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
  //                                                                                      Ogre::TEX_TYPE_2D,
  //                                                                                      RTT_PAINT_SIZE_X,
  //                                                                                      RTT_PAINT_SIZE_Y,
  //                                                                                      1,
  //                                                                                      Ogre::PF_R8G8B8A8,
  //                                                                                      Ogre::TU_RENDERTARGET);
  //        mCustomRenderTexturePaint->load();
  mRttPaint = mCustomRenderTexturePaint;
  const Ogre::String workspaceNamePaint = std::to_string(mRoot->getTimer()->getMicroseconds());
  const Ogre::IdString workspaceNamePaintHash = workspaceNamePaint;
  compositorManager->createBasicWorkspaceDef(workspaceNamePaint, Ogre::ColourValue::Black);
  mWorkspaceRttPaint =
      compositorManager->addWorkspace(mSceneManager, mRttPaint, mCamera, workspaceNamePaintHash, false);
}

//****************************************************************************/
void QOgreWidget::createSkyBoxCompositor()
{
  Ogre::CompositorManager2* compositorManager = mRoot->getCompositorManager2();
  mWorkspaceRttSkyBox =
      compositorManager->addWorkspace(mSceneManager, mOgreWindow->getTexture(), mCamera, SKYBOX_WORKSPACE, true);
  mWorkspaceRttSkyBox->setEnabled(false);
}

//****************************************************************************/
void QOgreWidget::updateOgre(Ogre::Real timeSinceLastFrame)
{
  if (!mSceneCreated)
  {
    createScene();
    mSceneCreated = true;
  }

  mTimeSinceLastFrame = timeSinceLastFrame;

  // Repaint
  repaint();
  if (this->size() != this->parentWidget()->size())
  {
    resize(this->parentWidget()->size());
  }

  if (mWorkspaceRttHoover)
  {
    // Set the background colour to black, otherwise the colours in the rtt are not uniquely assigned to a subItem
    // anymore
    Ogre::ColourValue c;
    bool toggleBackgroundColour = mBackground != Ogre::ColourValue::Black;
    if (toggleBackgroundColour)
    {
      c = mBackground;
      setBackgroundColour(Ogre::ColourValue::Black);
    }

    // Make items that not need to be rendered to the rtt to 'invisible'
    mSceneNode->setVisible(false);
    bool lightVisibility;
    bool skyBoxVisibility;
    if (mLightAxisItem)
    {
      lightVisibility = mLightAxisItem->getVisible();
      mLightAxisItem->setVisible(false);
    }

    // Make any skybox invisible
    skyBoxVisibility = mWorkspaceRttSkyBox->getEnabled();
    mWorkspaceRttSkyBox->setEnabled(false);

    // Update workspace hoover: begin
    mSceneNodeRttHoover->setVisible(true);
    mSceneNodeRttPaint->setVisible(false);
    mSceneManager->updateSceneGraph();  // Items made visible/invisble and if not added, the application crashes
    mWorkspaceRttHoover->_beginUpdate(true);
    mWorkspaceRttHoover->_update();
    mWorkspaceRttHoover->_endUpdate(true);
    // Update workspace: end

    // Update workspace paint: begin
    mSceneNodeRttHoover->setVisible(false);
    mSceneNodeRttPaint->setVisible(true);
    mWorkspaceRttPaint->_beginUpdate(true);
    mWorkspaceRttPaint->_update();
    mWorkspaceRttPaint->_endUpdate(true);
    // Update workspace: end

    // Reset the visibility
    mSceneNode->setVisible(true);
    mSceneNodeRttHoover->setVisible(false);  // Make the hoover and paint nodes invisible AFTER the item node
                                             // (mSceneNode) itself is set to visible This order is important to prevent
                                             // flickering of the renderwindow
    mSceneNodeRttPaint->setVisible(false);
    if (mLightAxisItem)
      mLightAxisItem->setVisible(lightVisibility);
    mWorkspaceRttSkyBox->setEnabled(skyBoxVisibility);

    if (toggleBackgroundColour)
      setBackgroundColour(c);
  }
}

//-------------------------------------------------------------------------------------
QPaintEngine* QOgreWidget::paintEngine() const
{
  // We don't want another paint engine to get in the way for our Ogre based paint engine.
  // So we return nothing.
  return 0;
}

//-------------------------------------------------------------------------------------
void QOgreWidget::paintEvent(QPaintEvent* e) {}

//-------------------------------------------------------------------------------------
void QOgreWidget::resizeEvent(QResizeEvent* e)
{
  if (e->isAccepted())
  {
    mSize = e->size();
    if (mCamera && mOgreWindow)
    {
      //                mOgreWindow->resize(mSize.width(), mSize.height());
      mOgreWindow->windowMovedOrResized();
      Ogre::Real aspectRatio = Ogre::Real(mSize.width()) / Ogre::Real(mSize.height());
      mCamera->setAspectRatio(aspectRatio);
    }
  }
}

//****************************************************************************/
void QOgreWidget::keyPressEvent(QKeyEvent* ev)
{
  if (mSystemInitialized)
  {
    mCameraManager->injectKeyDown(ev);
    if (ev->key() == Qt::Key_Shift)
      mShiftDown = true;
    else if (ev->key() == Qt::Key_A)
    {
      //                mRenderwindowDockWidget->handleTogglePaintMode();
      //                mRenderwindowDockWidget->enterEventPublic(0);
    }

    // Testcode to write the render-texture to a file
    if (ev->key() == Qt::Key_S)
      mRttHoover->writeContentsToFile("rtt_hoover.png", 100, 300);

    //#ifdef CREATE_UV_MAPPING_TEXTURE
    if (ev->key() == Qt::Key_P)
      mRttPaint->writeContentsToFile("rtt_paint.png", 100, 300);
    //#endif
  }
}

//****************************************************************************/
void QOgreWidget::keyReleaseEvent(QKeyEvent* ev)
{
  if (mSystemInitialized)
  {
    mCameraManager->injectKeyUp(ev);
    if (ev->key() == Qt::Key_Shift)
      mShiftDown = false;
  }
}

//****************************************************************************/
void QOgreWidget::enableLightItem(bool enabled)
{
  if (mLightAxisItem)
  {
    mRotateCameraMode = !enabled;  // We want to rotate the light if enabled = true
    mLightAxisItem->setVisible(enabled);
  }
}

//****************************************************************************/
void QOgreWidget::setHoover(bool hoover)
{
  mHoover = hoover;
  resetHighlight();
}

//****************************************************************************/
void QOgreWidget::setPaintMode(bool enabled) { mPaintMode = enabled; }

//****************************************************************************/
void QOgreWidget::setOffsetTextureMode(bool enabled,
                                       const Ogre::IdString& pbsDatablockBlockOffsetTexture,
                                       const Ogre::IdString& unlitDatablockBlockOffsetTexture,
                                       Ogre::PbsTextureTypes pbsTextureTypeOffsetTexture,
                                       Ogre::uint8 unlitTextureTypeOffsetTexture)
{
  mOffsetTextureMode = enabled;
  mPbsDatablockBlockOffsetTexture = pbsDatablockBlockOffsetTexture;
  mUnlitDatablockBlockOffsetTexture = unlitDatablockBlockOffsetTexture;
  mPbsTextureTypeOffsetTexture = pbsTextureTypeOffsetTexture;
  mUnlitTextureTypeOffsetTexture = unlitTextureTypeOffsetTexture;
}

//****************************************************************************/
void QOgreWidget::mouseMoveEvent(QMouseEvent* e)
{
  if (mSystemInitialized)
  {
    Ogre::Vector2 oldPos = mAbsolute;
    mAbsolute = Ogre::Vector2(e->pos().x(), e->pos().y());
    if (mMouseDown)
    {
      if (mPaintMode)
      {
        //                    doPaintLayer(e->pos().x(), e->pos().y());
        e->accept();
        return;
      }
      else if (mOffsetTextureMode)
      {
        mRelative = mAbsolute - oldPos;
        doOffsetTexture(mRelative.x, mRelative.y);
        e->accept();
        return;
      }

      if (mResetMousePos)
      {
        // Prevent that mRelative becomes to large, so rotation/position isn't smooth
        mRelative = Ogre::Vector2::ZERO;
        mResetMousePos = false;
      }
      else
      {
        mRelative = mAbsolute - oldPos;
      }

      if (mRotateCameraMode)
      {
        mCameraManager->injectMouseMove(mRelative);
        //                    mRenderwindowDockWidget->setCameraOrientationChanged
        //                    (mCameraManager->getOrientationCameraSceneNode());
        //                    mRenderwindowDockWidget->setCameraPositionChanged
        //                    (mCameraManager->getPositionCameraSceneNode());
      }
      else
        rotateLight(mRelative);
    }

    // Determine over which subItem the mouse hoovers
    if (mHoover)
      highlightSubItem(mAbsolute);
  }

  e->accept();
}

//****************************************************************************/
void QOgreWidget::rotateLight(Ogre::Vector2 relativeMouseMove)
{
  if (mMouseDown)
  {
    mLightAxisNode->roll(Ogre::Degree(relativeMouseMove.x * 0.25f));
    mLightAxisNode->pitch(Ogre::Degree(relativeMouseMove.y * 0.25f));
    mLight->setDirection(mLightAxisNode->getOrientation().yAxis());  // Light direction follows light axis
  }
}

//****************************************************************************/
void QOgreWidget::wheelEvent(QWheelEvent* e)
{
  if (mSystemInitialized)
  {
    mCameraManager->injectMouseWheel(e);
    mLightAxisNode->setPosition(mCamera->getPosition() + Ogre::Vector3(0, -27, -100));
  }
}

//****************************************************************************/
void QOgreWidget::mousePressEvent(QMouseEvent* e)
{
  if (mSystemInitialized)
  {
    mCameraManager->injectMouseDown(e);

    if (e->button() == Qt::LeftButton)
    {
      mMouseDown = true;
      if (mPaintMode)
      {
        //                    Ogre::Vector2 pos = Ogre::Vector2::ZERO;
        //                    mLatestPaintResult = doPaintLayer(e->pos().x(), e->pos().y(), true);
        //                    switch (mLatestPaintResult)
        //                    {
        //                        case 2:
        //                            QMessageBox::information(0, QString("Warning"), QString("Cannot paint. You haven't
        //                            created any paint layers."));
        //                        break;

        //                        case 3:
        //                            QMessageBox::information(0, QString("Warning"), QString("Cannot paint. There is no
        //                            material assigned to this part of the model."));
        //                        break;

        //                        case 4:
        //                            QMessageBox::information(0, QString("Warning"), QString("Cannot paint. Did you
        //                            forget to set the texture type in the paint layer?"));
        //                        break;

        //                        case 5:
        //                            QMessageBox::information(0, QString("Warning"), QString("Cannot paint. The current
        //                            material is not assigned to that part of the model"));
        //                        break;
        //                    }
      }
      else if (mOffsetTextureMode)
      {
        // TODO
      }
    }
    else if (e->button() == Qt::RightButton)
    {
      // Forward it to the render window dockwidget
      //                mRenderwindowDockWidget->mousePressEventPublic(e);
    }
  }
}

//****************************************************************************/
void QOgreWidget::mouseReleaseEvent(QMouseEvent* e)
{
  if (mSystemInitialized)
  {
    mCameraManager->injectMouseUp(e);
    mResetMousePos = true;
    if (e->button() == Qt::LeftButton)
    {
      mMouseDown = false;

      if (mPaintMode)
      {
        //                    // Only if the latest result was 0, the painting was successful; save the image
        //                    if (mLatestPaintResult == 0)
        //                        doPaintSaveTextureGeneration();

        //                    mLatestPaintResult = 1;
      }
      else if (mOffsetTextureMode)
      {
        // TODO
      }
    }
  }
  e->accept();
}

//****************************************************************************/
void QOgreWidget::mouseDoubleClickEvent(QMouseEvent* event) { assignCurrentDatablock(); }

//****************************************************************************/
void QOgreWidget::enterEvent(QEvent* event)
{
  //        mRenderwindowDockWidget->enterEventPublic (event); // The render dockwindow needs it
}

//****************************************************************************/
void QOgreWidget::leaveEvent(QEvent* event)
{
  //        mRenderwindowDockWidget->leaveEventPublic (event); // The render dockwindow needs it
}

//****************************************************************************/
void QOgreWidget::assignCurrentDatablock()
{
  if (mSystemInitialized)
  {
    int index = mLatestSubItemIndexHighlighted;
    if (index > -1)
      resetHighlight();

    if (mHoover)
    {
      if (index > -1)
        mItem->getSubItem(index)->setDatablock(mCurrentDatablockId);
    }
    else
      mItem->setDatablock(mCurrentDatablockId);
  }
}

//****************************************************************************/
int QOgreWidget::getIndexOfHighlightedSubmesh(void) { return mLatestSubItemIndexHighlighted; }

//****************************************************************************/
const Ogre::String& QOgreWidget::getDatablockNameOfHighlightedSubmesh(void)
{
  // Retrieve the latest datablock instead of the current datablock, because the mouse may be hoovering
  // over the submesh, meaning that the 'green' highlighted datablock is returned instead; this is not
  // what we want.
  mHelperString = "";
  if (!mItem || !mLatestSubItemDatablock)
    return mHelperString;

  if (mLatestSubItemIndexHighlighted == -1)
    return mHelperString;

  if (mLatestSubItemIndexHighlighted >= 0 && mLatestSubItemIndexHighlighted >= mItem->getNumSubItems())
    return mHelperString;

  mHelperString = Ogre::String(*mLatestSubItemDatablock->getNameStr());
  return mHelperString;
}

//****************************************************************************/
const Ogre::IdString& QOgreWidget::getDatablockOfHighlightedSubmesh(void)
{
  // Retrieve the latest datablock instead of the current datablock, because the mouse may be hoovering
  // over the submesh, meaning that the 'green' highlighted datablock is returned instead; this is not
  // what we want.
  mHelperIdString = "";
  if (!mItem || !mLatestSubItemDatablock)
    return mHelperIdString;

  if (mLatestSubItemIndexHighlighted == -1)
    return mHelperIdString;

  if (mLatestSubItemIndexHighlighted >= 0 && mLatestSubItemIndexHighlighted >= mItem->getNumSubItems())
    return mHelperIdString;

  mHelperIdString = mLatestSubItemDatablock->getName();
  return mHelperIdString;
}

//****************************************************************************/
const Ogre::Vector3 QOgreWidget::getItemScale(void)
{
  if (mItem && mItem->getParentSceneNode())
    return mItem->getParentSceneNode()->getScale();

  // Need to return on all code paths
  return Ogre::Vector3::UNIT_SCALE;  // vec3{1,1,1}
}

//****************************************************************************/
void QOgreWidget::setItemScale(const Ogre::Vector3& scale)
{
  if (mItem && mItem->getParentSceneNode())
    mItem->getParentSceneNode()->setScale(scale);
}

//****************************************************************************/
void QOgreWidget::setRotation(const Ogre::Vector3& rotation)
{
  // Rotation consists of euler angle rotation
  mCameraManager->rotate(rotation);
  mResetMousePos = true;  // To prevent that the camera shakes when the mouse is used for rotation
}

//****************************************************************************/
void QOgreWidget::setPosition(const Ogre::Vector3& position)
{
  mCameraManager->pan(position);
  mResetMousePos = true;  // To prevent that the camera shakes when the mouse is moved
}

//****************************************************************************/
void QOgreWidget::saveToFile(const Ogre::String& fileName)
{
  mOgreWindow->getTexture()->writeContentsToFile(fileName, 100, 300);
}

//****************************************************************************/
const Ogre::ColourValue& QOgreWidget::getColourAtRenderToTextureHoover(int x, int y)
{
  // Sometimes the mousecoordinates are beyond the renderwindow. To prevent crashes in
  // pixelbox.getColourAt the maximum values of the mousecoordinates are validated
  x = ((float)x / (float)mSize.width()) * RTT_HOOVER_SIZE_X;
  y = ((float)y / (float)mSize.height()) * RTT_HOOVER_SIZE_Y;
  mHelpColour = Ogre::ColourValue::Black;
  if (x > RTT_HOOVER_SIZE_X || y > RTT_HOOVER_SIZE_Y)
    return mHelpColour;
  // TODO Levi figure out if this is need and if so fix
  //        size_t formatSize = Ogre::PixelUtil::getNumElemBytes(Ogre::PF_R8G8B8A8);
  //        Ogre::uchar* data = OGRE_ALLOC_T(Ogre::uchar, RTT_HOOVER_SIZE_X * RTT_HOOVER_SIZE_Y * formatSize,
  //        Ogre::MEMCATEGORY_RENDERSYS); Ogre::PixelBox pixelbox (RTT_HOOVER_SIZE_X, RTT_HOOVER_SIZE_Y, 1,
  //        Ogre::PF_R8G8B8A8, data); mRttHoover->copyContentsToMemory(pixelbox, Ogre::RenderTarget::FB_AUTO);
  //        mHelpColour = pixelbox.getColourAt(x, y, 0);
  //        OGRE_FREE(data, Ogre::MEMCATEGORY_RENDERSYS);
  return mHelpColour;
}

//****************************************************************************/
void QOgreWidget::highlightSubItem(Ogre::Vector2 mousePos)
{
  Ogre::ColourValue colour = getColourAtRenderToTextureHoover(
      (int)mousePos.x, (int)mousePos.y);  // Get the colour of the mouse position (from the render texture)
  int index =
      calculateColourToIndex(colour);  // Get the index of the subitem, based on the colour at the mouse position

  // Determine whether index is out of bounds (because of the symplistic scalar to colour mapping algorithm)
  if (index >= 0 && index >= mItem->getNumSubItems())
    return;

  // Determine whether the mouse is still over the latest subitem
  if (index == mLatestSubItemIndexHighlighted)
    return;

  // Determine whether index is still on a subitem
  if (index < 0)
  {
    if (mLatestSubItemIndexHighlighted >= 0)
    {
      // The mouse is currently not pointing to any subitem, but was previously pointing to a subitem, so restore the
      // material
      resetHighlight();
    }
    else
    {
      // The mouse is currently not pointing to any subitem and was previously also not pointing to a subitem
      return;
    }
  }
  else
  {
    if (mLatestSubItemIndexHighlighted >= 0)
    {
      // The mouse is pointing to a subitem and previously pointing to another subitem, so restore the material
      resetHighlight();
    }

    // Highlight the subItem
    mLatestSubItemIndexHighlighted = index;
    setHighlightDatablockToSubItem(index);
  }
}

//****************************************************************************/
int QOgreWidget::getSubItemIndexWithMouseOver(int mouseX, int mouseY)
{
  Ogre::ColourValue colour = getColourAtRenderToTextureHoover(mouseX, mouseY);  // Get the colour of the mouse position
                                                                                // (from the render texture)
  int index =
      calculateColourToIndex(colour);  // Get the index of the subitem, based on the colour at the mouse position

  // Determine whether index is out of bounds
  if (index >= 0 && index >= mItem->getNumSubItems())
    return -1;

  return index;
}

//****************************************************************************/
void QOgreWidget::setHighlightDatablockToSubItem(int index)
{
  mLatestSubItemDatablock = mItem->getSubItem(index)->getDatablock();
  mItem->getSubItem(index)->setDatablock(HIGHLIGHT_MATERIAL_NAME);
}

//****************************************************************************/
void QOgreWidget::resetHighlight(void)
{
  if (!mItem || !mLatestSubItemDatablock)
    return;

  if (mLatestSubItemIndexHighlighted == -1)
    return;

  if (mLatestSubItemIndexHighlighted >= 0 && mLatestSubItemIndexHighlighted >= mItem->getNumSubItems())
    return;

  mItem->getSubItem(mLatestSubItemIndexHighlighted)->setDatablock(mLatestSubItemDatablock->getName());
  mLatestSubItemIndexHighlighted = -1;
  mLatestSubItemDatablock = 0;
}

//****************************************************************************/
void QOgreWidget::resetCamera(void)
{
  mCamera->setPosition(Ogre::Vector3(0, 40, 145));
  mCamera->lookAt(Ogre::Vector3(0, 0, 0));
  mCamera->getParentSceneNode()->setOrientation(Ogre::Quaternion::IDENTITY);
  mLightAxisNode->setPosition(mCamera->getPosition() + Ogre::Vector3(0, -27, -100));
  mLightAxisNode->setOrientation(Ogre::Quaternion::IDENTITY);
  mSceneNode->setPosition(0.0, 0.0, 0.0);
  mSceneNodeRttHoover->setPosition(0.0, 0.0, 0.0);
  mSceneNodeRttPaint->setPosition(0.0, 0.0, 0.0);
  mCameraManager->resetCameraNode();
}

//****************************************************************************/
void QOgreWidget::setCurrentDatablockId(const Ogre::IdString& datablockId) { mCurrentDatablockId = datablockId; }

//****************************************************************************/
const QVector<int>& QOgreWidget::getSubItemIndicesWithDatablock(const Ogre::IdString& datablockId)
{
  helperIndices.clear();
  size_t numSubItems = mItem->getNumSubItems();
  Ogre::SubItem* subItem;
  Ogre::HlmsDatablock* datablock;

  for (size_t i = 0; i < numSubItems; ++i)
  {
    subItem = mItem->getSubItem(i);
    datablock = subItem->getDatablock();
    if (datablock->getName() == datablockId)
      helperIndices.append(i);
  }

  return helperIndices;
}

//****************************************************************************/
void QOgreWidget::makeSnapshotOfItemMaterials(void)
{
  mSnapshotDatablocks.clear();
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
  Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));
  size_t numSubItems = mItem->getNumSubItems();
  Ogre::SubItem* subItem;
  Ogre::HlmsDatablock* datablock;
  Ogre::String datablockNameStr;

  for (size_t i = 0; i < numSubItems; ++i)
  {
    subItem = mItem->getSubItem(i);
    datablock = subItem->getDatablock();
    if (datablock)
    {
      datablockNameStr = *datablock->getNameStr();
      // Exclude special datablocks
      if (datablock != hlmsPbs->getDefaultDatablock() && datablock != hlmsUnlit->getDefaultDatablock() &&
          datablockNameStr != tesseract_gui::DEFAULT_DATABLOCK_NAME &&
          datablockNameStr != tesseract_gui::AXIS_MATERIAL_NAME &&
          datablockNameStr != tesseract_gui::HIGHLIGHT_MATERIAL_NAME &&
          datablockNameStr != tesseract_gui::UV_MAPPING_MATERIAL_NAME)
      {
        mSnapshotDatablocks[i] = datablockNameStr;
      }
    }
  }
}

//****************************************************************************/
void QOgreWidget::restoreSnapshotOfItemMaterials(void)
{
  QMap<size_t, Ogre::String>::iterator it = mSnapshotDatablocks.begin();
  QMap<size_t, Ogre::String>::iterator itEnd = mSnapshotDatablocks.end();
  size_t index;
  Ogre::String datablockNameStr;
  Ogre::HlmsDatablock* datablock;

  while (it != itEnd)
  {
    datablockNameStr = it.value();
    index = it.key();
    datablock = getDatablockByNameStr(datablockNameStr);
    Ogre::SubItem* subItem;
    if (mItem && datablock)
    {
      subItem = mItem->getSubItem(index);
      if (subItem)
      {
        try
        {
          // Ogre::LogManager::getSingleton().logMessage("make snaphot: " +  *datablock->getNameStr()); // DEBUG
          subItem->setDatablock(datablock->getName());
        }
        catch (Ogre::Exception e)
        {
        }
      }
      // Ogre::LogManager::getSingleton().logMessage("restore snaphot: " +  datablockNameStr); // DEBUG
    }
    ++it;
  }
}

//****************************************************************************/
Ogre::HlmsDatablock* QOgreWidget::getDatablockByNameStr(const Ogre::String& nameStr)
{
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
  Ogre::HlmsPbs* hlmsUnlit = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));

  Ogre::Hlms::HlmsDatablockMap::const_iterator itorPbs = hlmsPbs->getDatablockMap().begin();
  Ogre::Hlms::HlmsDatablockMap::const_iterator endPbs = hlmsPbs->getDatablockMap().end();
  Ogre::HlmsDatablock* datablock;
  while (itorPbs != endPbs)
  {
    datablock = itorPbs->second.datablock;
    if (datablock && *datablock->getNameStr() == nameStr)
      return datablock;

    ++itorPbs;
  }

  Ogre::Hlms::HlmsDatablockMap::const_iterator itorUnlit = hlmsUnlit->getDatablockMap().begin();
  Ogre::Hlms::HlmsDatablockMap::const_iterator endUnlit = hlmsUnlit->getDatablockMap().end();
  while (itorUnlit != endUnlit)
  {
    datablock = itorUnlit->second.datablock;
    if (datablock && *datablock->getNameStr() == nameStr)
      return datablock;

    ++itorUnlit;
  }

  return 0;
}

//****************************************************************************/
Ogre::MeshPtr QOgreWidget::getCurrentMeshEnrichedWithItemDatablocksNameStr(void)
{
  Ogre::MeshPtr mesh = mItem->getMesh();
  Ogre::Mesh* meshPtr = mesh.getPointer();
  size_t numSubItems = mItem->getNumSubItems();
  Ogre::SubItem* subItem;
  Ogre::HlmsDatablock* datablock;
  Ogre::String datablockNameStr;

  for (size_t i = 0; i < numSubItems; ++i)
  {
    subItem = mItem->getSubItem(i);
    datablock = subItem->getDatablock();
    if (datablock)
    {
      datablockNameStr = *datablock->getNameStr();
      meshPtr->getSubMesh(i)->setMaterialName(datablockNameStr);
    }
  }

  return mItem->getMesh();
}

//****************************************************************************/
Ogre::MeshPtr QOgreWidget::getCurrentMesh(void) { return mItem->getMesh(); }

//****************************************************************************/
const QMap<unsigned short, Ogre::String>& QOgreWidget::getMaterialNamesFromCurrentMesh(void)
{
  // Iterate through the current mesh and return a map with materialnames
  helperIndicesAndNames.clear();
  if (mItem)
  {
    Ogre::MeshPtr mesh;
    Ogre::Mesh* meshPtr;
    Ogre::SubMesh* subMesh;
    size_t numSubMeshes;
    mesh = mItem->getMesh();
    meshPtr = mesh.getPointer();
    numSubMeshes = meshPtr->getNumSubMeshes();
    for (size_t i = 0; i < numSubMeshes; ++i)
    {
      subMesh = meshPtr->getSubMesh(i);
      helperIndicesAndNames[i] = subMesh->getMaterialName();
    }
  }

  return helperIndicesAndNames;
}

//****************************************************************************/
void QOgreWidget::setDatablockInSubItem(int index, const Ogre::IdString datablockId)
{
  if (mItem)
  {
    Ogre::SubItem* subItem = mItem->getSubItem(index);
    if (subItem)
      subItem->setDatablock(datablockId);
  }
}

//****************************************************************************/
void QOgreWidget::setDatablockInSubItems(const QVector<int>& indices, const Ogre::IdString& datablockId)
{
  int index;
  Ogre::SubItem* subItem;
  QVectorIterator<int> it(indices);
  while (it.hasNext())
  {
    index = it.next();
    subItem = mItem->getSubItem(index);
    subItem->setDatablock(datablockId);
  }

  mCurrentDatablockId = datablockId;
}

//****************************************************************************/
void QOgreWidget::setRenderwindowDockWidget(RenderwindowDockWidget* renderwindowDockWidget)
{
  // This is needed to be able to forward the right mousepress to the RenderwindowDockWidget
  //        mRenderwindowDockWidget = renderwindowDockWidget;
}

//****************************************************************************/
void QOgreWidget::doOffsetTexture(float relativeMouseX, float relativeMouseY)
{
  Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();
  if (mPbsDatablockBlockOffsetTexture != "")
  {
    Ogre::uint8 index = getDetailMapIndexFromTextureTypeForScaleAndOffset(mPbsTextureTypeOffsetTexture);
    if (index < 999)
    {
      Ogre::HlmsPbs* hlmsPbs = static_cast<Ogre::HlmsPbs*>(hlmsManager->getHlms(Ogre::HLMS_PBS));
      Ogre::HlmsPbsDatablock* pbsDatablock =
          static_cast<Ogre::HlmsPbsDatablock*>(hlmsPbs->getDatablock(mPbsDatablockBlockOffsetTexture));
      Ogre::Vector4 v4 = pbsDatablock->getDetailMapOffsetScale(index);
      v4.x += relativeMouseX / (float)width();
      v4.x = saturate(v4.x);
      v4.y += relativeMouseY / (float)height();
      v4.y = saturate(v4.y);
      pbsDatablock->setDetailMapOffsetScale(index, v4);
      //                mRenderwindowDockWidget->notifyOffsetTextureUpdated(v4.x, v4.y);
    }
  }
  else if (mUnlitDatablockBlockOffsetTexture != "")
  {
    Ogre::HlmsUnlit* hlmsUnlit = static_cast<Ogre::HlmsUnlit*>(hlmsManager->getHlms(Ogre::HLMS_UNLIT));
    Ogre::HlmsUnlitDatablock* unlitDatablock =
        static_cast<Ogre::HlmsUnlitDatablock*>(hlmsUnlit->getDatablock(mUnlitDatablockBlockOffsetTexture));
    Ogre::Matrix4 m4 = unlitDatablock->getAnimationMatrix(mUnlitTextureTypeOffsetTexture);
    Ogre::Vector3 v3 = m4.getTrans();
    v3.x += relativeMouseX / (float)width();
    v3.x = saturate(v3.x);
    v3.y += relativeMouseY / (float)height();
    v3.y = saturate(v3.y);
    m4.setTrans(v3);
    unlitDatablock->setAnimationMatrix(mUnlitTextureTypeOffsetTexture, m4);
    unlitDatablock->setEnableAnimationMatrix(mUnlitTextureTypeOffsetTexture, true);
    //            mRenderwindowDockWidget->notifyOffsetTextureUpdated(v3.x, v3.y);
  }
}

//****************************************************************************/
float QOgreWidget::saturate(float val)
{
  val = val > 1.0f ? 1.0f : (val < 0.0f ? 0.0f : val);
  return val;
}

//****************************************************************************/
Ogre::uint8 QOgreWidget::getDetailMapIndexFromTextureTypeForScaleAndOffset(Ogre::PbsTextureTypes textureType)
{
  switch (textureType)
  {
    case Ogre::PBSM_DETAIL0:
      return 0;
      break;
    case Ogre::PBSM_DETAIL1:
      return 1;
      break;
    case Ogre::PBSM_DETAIL2:
      return 2;
      break;
    case Ogre::PBSM_DETAIL3:
      return 3;
      break;
    case Ogre::PBSM_DETAIL0_NM:
      return 4;
      break;
    case Ogre::PBSM_DETAIL1_NM:
      return 5;
      break;
    case Ogre::PBSM_DETAIL2_NM:
      return 6;
      break;
    case Ogre::PBSM_DETAIL3_NM:
      return 7;
      break;
  }
  return 999;
}

//****************************************************************************/
//    void QOgreWidget::setPaintLayers(PaintLayers* paintLayers)
//    {
//        mPaintLayers = paintLayers;
//    }

//    //****************************************************************************/
//    int QOgreWidget::doPaintLayer(int mouseX, int mouseY, bool start)
//    {
//        // Only paint in case:
//        // - The paint layer is enabled
//        // - The paint layer is visible
//        // - The datablock of the submesh is equal to datablock of the paint layer
//        // - The texture type is set in all paintlayers
//        // - The material on the item must be set with the current material in the editor

//        if (!mItem)
//            return 1;

//        // If the mouse didn't hoover over a submesh, don't paint (this can be done quickly by using the RTT mechanism
//        that is already used for mouse hoovering) int index = getSubItemIndexWithMouseOver(mouseX, mouseY); if (index
//        < 0)
//            return 1;

//        // If there are no paintlayers, don't continue
//        if (!mPaintLayers || mPaintLayers->size() == 0)
//            return 2;

//        // The subitem must have a valid datablock; otherwise don't paint
//        Ogre::HlmsDatablock* datablock = mItem->getSubItem(index)->getDatablock();
//        if (!datablock || datablock->getName() == DEFAULT_DATABLOCK_NAME)
//            return 3;

//        float u = 0.5f;
//        float v = 0.5f;
//        bool uvCalculated = false;
//        Ogre::ColourValue col;

//        // Iterate through the PaintLayer vector and apply the paint effect
//        PaintLayers::iterator it;
//        PaintLayers::iterator itStart = mPaintLayers->begin();
//        PaintLayers::iterator itEnd = mPaintLayers->end();
//        PaintLayer* paintLayer;
//        for (it = itStart; it != itEnd; ++it)
//        {
//            paintLayer = *it;
//            if (paintLayer->isEnabled() && paintLayer->isVisible())
//            {
//                if (!paintLayer->getTextureLayer() || !paintLayer->getTextureLayer()->mTextureTypeDefined)
//                    return 4;

//                if (paintLayer->getDatablockId() != datablock->getName())
//                    return 5;

//                // Calculate the uv; we do it here, because it should only be calculated when needed (as late as
//                possible) if (!uvCalculated)
//                {
//                    // Calculate the uv
//                    col = getColourAtRenderToTexturePaint(mouseX, mouseY);
//                    helperVector2 = calculateColourToUv(col);
//                    u = helperVector2.x;
//                    v = helperVector2.y;
//                    uvCalculated = true;
//                }

//                paintLayer->paint(u, v, start);
//            }
//        }

//        return 0;
//    }

//****************************************************************************/
//    void QOgreWidget::doPaintSaveTextureGeneration (void)
//    {
//        // Iterate through the PaintLayer vector and apply the paint effect
//        PaintLayers::iterator it;
//        PaintLayers::iterator itStart = mPaintLayers->begin();
//        PaintLayers::iterator itEnd = mPaintLayers->end();
//        PaintLayer* paintLayer;
//        TextureLayer* textureLayer;
//        Ogre::ushort sequence;
//        Ogre::PbsTextureTypes textureType;
//        for (it = itStart; it != itEnd; ++it)
//        {
//            paintLayer = *it;
//            if (paintLayer->isEnabled() && paintLayer->isVisible())
//            {
//                // Save the latest image
//                paintLayer->saveTextureGeneration();

//                // Add the texture type and sequence number to the UndoRedoStack.
//                // If the sequence number is 1, add the base texture (sequence 0) to the UndoRedoStack first
//                textureLayer = paintLayer->getTextureLayer();
//                if (textureLayer && textureLayer->mTextureTypeDefined)
//                {
//                    sequence = textureLayer->mMaxSequence;
//                    textureType = textureLayer->mTextureType;
//                    if (sequence == 1)
//                    {
//                        // First add the base texture (the first one)
////                        mRenderwindowDockWidget->addUndoRedoStackEntry(textureType, 0);
//                    }
////                    mRenderwindowDockWidget->addUndoRedoStackEntry(textureType, sequence);
//                }
//            }
//        }
//    }

//****************************************************************************/
const Ogre::ColourValue& QOgreWidget::getColourAtRenderToTexturePaint(int x, int y)
{
  //        // Sometimes the mousecoordinates are beyond the renderwindow. To prevent crashes in
  //        // pixelbox.getColourAt the maximum values of the mousecoordinates are validated
  //        x = ((float)x / (float)mSize.width()) * RTT_PAINT_SIZE_X;
  //        y = ((float)y / (float)mSize.height()) * RTT_PAINT_SIZE_Y;
  //        mHelpColour = Ogre::ColourValue::Black;
  //        if (x > RTT_PAINT_SIZE_X || y > RTT_PAINT_SIZE_Y)
  //            return mHelpColour;

  //        size_t formatSize = Ogre::PixelUtil::getNumElemBytes(Ogre::PF_R8G8B8A8);
  //        Ogre::uchar* data = OGRE_ALLOC_T(Ogre::uchar, RTT_PAINT_SIZE_X * RTT_PAINT_SIZE_Y * formatSize,
  //        Ogre::MEMCATEGORY_RENDERSYS); Ogre::PixelBox pixelbox (RTT_PAINT_SIZE_X, RTT_PAINT_SIZE_Y, 1,
  //        Ogre::PF_R8G8B8A8, data); mRttPaint->copyContentsToMemory(pixelbox, Ogre::RenderTarget::FB_AUTO);
  //        mHelpColour = pixelbox.getColourAt(x, y, 0);
  //        OGRE_FREE(data, Ogre::MEMCATEGORY_RENDERSYS);
  //        return mHelpColour;
}

//****************************************************************************/
void QOgreWidget::removeDatablockFromItem(const Ogre::IdString datablockId)
{
  size_t numSubItems = mItem->getNumSubItems();
  Ogre::SubItem* subItem;
  Ogre::HlmsDatablock* datablock;

  for (size_t i = 0; i < numSubItems; ++i)
  {
    subItem = mItem->getSubItem(i);
    datablock = subItem->getDatablock();
    if (datablock->getName() == datablockId)
      setDefaultDatablockSubItem(subItem);
  }
}
}  // namespace tesseract_gui
