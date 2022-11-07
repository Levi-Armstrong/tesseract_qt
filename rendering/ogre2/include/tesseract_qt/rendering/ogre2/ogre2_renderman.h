/****************************************************************************
**
** Copyright (C) 2016 - 2017
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
#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_RENDERMAN_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_RENDERMAN_H

#include <tesseract_qt/rendering/ogre2/ogre2_widget.h>
#include <tesseract_qt/rendering/ogre2/ogre2_prereqs.h>
#include <OgreRoot.h>
#include <OgreWindow.h>
#include <OgreRenderSystem.h>
#include <OgreString.h>
#include <OgreTimer.h>
#include <OgreColourValue.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/Pass/OgreCompositorPassProvider.h>

namespace Ogre
{
class MyCompositorPassProvider : public CompositorPassProvider
{
public:
  virtual CompositorPassDef* addPassDef(CompositorPassType passType,
                                        IdString customId,
                                        CompositorTargetDef* parentTargetDef,
                                        CompositorNodeDef* parentNodeDef)
  {
    return 0;
  }

  virtual CompositorPass* addPass(const CompositorPassDef* definition,
                                  Camera* defaultCamera,
                                  CompositorNode* parentNode,
                                  const CompositorChannel& target,
                                  SceneManager* sceneManager)
  {
    return 0;
  }
};
}  // namespace Ogre

namespace tesseract_gui
{
static const Ogre::String OGRE_RENDERSYSTEM_DIRECTX11 = "Direct3D11 Rendering Subsystem";
static const Ogre::String OGRE_RENDERSYSTEM_OPENGL3PLUS = "OpenGL 3+ Rendering Subsystem";
static const Ogre::String OGRE_RENDERSYSTEM_METAL = "Metal Rendering Subsystem";

/****************************************************************************
 * This class manages the rendering of the Ogre widgets
 * Ogre Manager for Ogre > V2.0
 ***************************************************************************/
class OgreManager : public QObject
{
public:
  OgreManager(void);
  virtual ~OgreManager(void);
  void initialize(void);
  void renderOgreWidgetsOneFrame(void);
  void registerOgreWidget(int ogreWidgetId, QOgreWidget* ogreWidget);
  void unregisterOgreWidget(int ogreWidgetId);
  QOgreWidget* getOgreWidget(int ogreWidgetId) const;
  Ogre::Root* getOgreRoot(void);
  bool isRenderSystemGL(void);
  GL_CONTEXT getGlContext(void) const;
  void setGlContext(GL_CONTEXT glContext);
  void setPause(bool pause);

private:
  QMap<int, QOgreWidget*> mQOgreWidgetMap;
  Ogre::Root* mRoot;
  Ogre::Timer* mTimer;
  Ogre::String mResourcesCfg;
  Ogre::String mPluginsCfg;
  Ogre::RenderSystem* mCurrentRenderSystem;
  void setupResources(void);
  void registerHlms(void);
  void registerHlmsOriginal(void);
  GL_CONTEXT mGlContext;
  Ogre::MyCompositorPassProvider* mCompositorPassProvider;
  bool mPause;
};
}  // namespace tesseract_gui

//****************************************************************************/
#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_RENDERMAN_H
