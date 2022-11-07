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
#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_WIDGET_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_WIDGET_H

#include <QWidget>
#include <QPaintEngine>
#include <OgreRoot.h>
#include <OgreWindow.h>
#include <OgreString.h>
#include <OgreCamera.h>
#include <OgreColourValue.h>
#include <OgreVector2.h>
#include <OgreItem.h>
#include <OgreHlmsPbsDatablock.h>
#include <OgreHlmsUnlitDatablock.h>
//#include "ogre_prereqs.h"
//#include <QDockWidget>
#include <tesseract_qt/rendering/ogre2/ogre2_prereqs.h>
#include <tesseract_qt/rendering/ogre2/ogre2_cameraman.h>

// Set the next line to comment, otherwise the uv mapping texture will be created
// at runtime, which is not what we need. It is only for convenience to create the
// uv mapping texture once.
//#define CREATE_UV_MAPPING_TEXTURE 1

class RenderwindowDockWidget;
namespace tesseract_gui
{
static const Ogre::String AXIS_MATERIAL_NAME = "AMN0894587568";
static const Ogre::String HIGHLIGHT_MATERIAL_NAME = "HLM1234567890";
static const Ogre::String UV_MAPPING_MATERIAL_NAME = "UVMM5432123456";
static const Ogre::String SKYBOX_WORKSPACE = "SkyPostprocessWorkspace";
static const Ogre::String SKYBOX_MATERIAL_NAME = "SkyPostprocess";
static const Ogre::String OGRE3_PATH = "../common/ogre3/";
static const Ogre::String UV_MAPPING_TEXTURE_NAME = "uv_mapping.png";
static const int MAX_SCREEN_SIZE_INT =
    10000;  // Arbritrary value, but screen size is usually not larger than this, right?

class OgreManager;

/****************************************************************************
QT Ogre Window for Ogre > V2.0
***************************************************************************/
class QOgreWidget : public QWidget
{
  Q_OBJECT

public:
  QOgreWidget(QWidget* parent = 0);
  virtual ~QOgreWidget(void);
  QPaintEngine* paintEngine() const;  // Turn off QTs paint engine for the Ogre widget.
  void updateOgre(float timeSinceLastFrame);
  void createRenderWindow(OgreManager* ogreManager);
  void createScene();
  Ogre::Item* getItem(void) { return mItem; }
  Ogre::Window* getRenderWindow(void) { return mOgreWindow; }
  Ogre::SceneManager* getSceneManager(void) { return mSceneManager; }
  const Ogre::Vector3 getItemScale(void);
  void setItemScale(const Ogre::Vector3& scale);
  void setRotation(const Ogre::Vector3& rotation);
  void setPosition(const Ogre::Vector3& position);
  void setDefaultDatablockItem(void);
  void setDefaultDatablockSubItem(Ogre::SubItem* subItem);
  void setDefaultDatablockInSubItem(int index);  // Set default datablock in sub item
  void setDefaultDatablockItemRttHoover(void);   // Set the mItem to default
  void saveToFile(const Ogre::String& fileName);

  // Create item and use the current datablock of the previous item (if available)
  void createItem(const Ogre::String& meshName, const Ogre::Vector3& scale);

  // Set the background colour
  void setBackgroundColour(const Ogre::ColourValue colour);

  void setRotateCameraMode(bool rotateCameraMode) { mRotateCameraMode = rotateCameraMode; }
  bool getRotateCameraMode(void) const { return mRotateCameraMode; }
  void enableLightItem(bool enabled);                                  // Used to switch to ' light rotation mode'
  void createLightAxisMaterial(void);                                  // Create the datablock of the light axis item
  void destroyLightAxisMaterial(void);                                 // Destroy the datablock of the light axis item
  void updateSkyBoxMaterial(const Ogre::String& cubeMapBaseFileName);  // Update the material of the skybox item
  void createHighlightMaterial(void);          // Create the datablock used to highlight a subItem of mItem
  void destroyHighlightMaterial(void);         // Destroy the datablock used to highlight a subItem of mItem
  void createUnlitDatablocksRttHoover(void);   // Create datablocks of mItemRttHoover
  void createUnlitDatablockRttPaint(void);     // Create datablock of mItemRttPaint
  void destroyUnlitDatablockRttPaint(void);    // Destroy datablock of mItemRttPaint
  void destroyUnlitDatablocksRttHoover(void);  // Destroy the datablocks of mItemRttHoover
  void resetHighlight(void);                   // Undo highlight of mItem (preserving its datablocks)
  void resetCamera(void);                      // Set position/orientation to default
  void setHoover(bool hoover);      // Determines whether the subItems are highlighted when the mousecursor hoovers over
                                    // them
  void setPaintMode(bool enabled);  // Determines whether paint mode is on or off

  // Determines whether moving tetures (displacement) mode is on or off
  // Either pbsDatablockBlockOffsetTexture is set or unlitDatablockBlockOffsetTexture is set in case enabled is true
  void setOffsetTextureMode(bool enabled,
                            const Ogre::IdString& pbsDatablockBlockOffsetTexture,
                            const Ogre::IdString& unlitDatablockBlockOffsetTexture,
                            Ogre::PbsTextureTypes pbsTextureTypeOffsetTexture = Ogre::PBSM_DIFFUSE,
                            Ogre::uint8 unlitTextureTypeOffsetTexture = 0);

  void setCurrentDatablockId(const Ogre::IdString& datablockId);  // Only set the new datablock id; the new datablock is
                                                                  // not set in an item/subitem
  const Ogre::IdString& getCurrentDatablockId(void) const { return mCurrentDatablockId; }
  const QVector<int>& getSubItemIndicesWithDatablock(const Ogre::IdString& datablockId);  // Get the list of indices of
                                                                                          // subItems that have
                                                                                          // 'datablockId'
  void makeSnapshotOfItemMaterials(void);                               // Keep a map of all materials per subItem
  void restoreSnapshotOfItemMaterials(void);                            // Assign the material to each subItem
  Ogre::MeshPtr getCurrentMeshEnrichedWithItemDatablocksNameStr(void);  // Returns the mesh associated with mItem, but
                                                                        // with the datablock namestring of the mItem
                                                                        // added to the mesh
  Ogre::MeshPtr getCurrentMesh(void);  // Returns the mesh associated with mItem, but with the original materialnames in
                                       // the mesh
  const QMap<unsigned short, Ogre::String>& getMaterialNamesFromCurrentMesh(void);  // Returns map with index (of
                                                                                    // subMeash) and its materialname
  void setDatablockInSubItem(int index, const Ogre::IdString datablockId);  // Set the datablock in the subitem, based
                                                                            // on the id (datablockId) of the datablock
  void setDatablockInSubItems(const QVector<int>& indices,
                              const Ogre::IdString& datablockId);  // Set the datablock in the subitems, identified by
                                                                   // indices
  void cleanup(void);
  void setRenderwindowDockWidget(RenderwindowDockWidget* renderwindowDockWidget);
  void assignCurrentDatablock(void);  // Set the current datablock in the item or subItem
  bool isSkyBoxVisible(void);
  void setSkyBoxVisible(bool visible);
  //            void setPaintLayers(PaintLayers* paintLayers); // Set pointer to vector with PaintLayers
  int getSubItemIndexWithMouseOver(int mouseX, int mouseY);  // Returns the subitem index of the subitem where the mouse
                                                             // hoovers over
  int getIndexOfHighlightedSubmesh(void);
  void removeDatablockFromItem(const Ogre::IdString id);
  const Ogre::String& getDatablockNameOfHighlightedSubmesh(void);  // Returns the name string of the datablock of the
                                                                   // highlighted submesh
  const Ogre::IdString& getDatablockOfHighlightedSubmesh(void);

protected:
  Ogre::Root* mRoot;
  Ogre::Window* mOgreWindow{ nullptr };
  Ogre::SceneManager* mSceneManager{ nullptr };
  Ogre::SceneManager* mSceneManagerTest{ nullptr };
  Ogre::Camera* mCamera{ nullptr };
  Ogre::ColourValue mBackground;
  Ogre::Real mTimeSinceLastFrame;
  CameraMan* mCameraManager{ nullptr };
  bool mSceneCreated;
  Ogre::Vector2 mAbsolute;
  Ogre::Vector2 mRelative;
  bool mSystemInitialized;
  Ogre::Item* mItem{ nullptr };
  Ogre::Item* mItemRttHoover{ nullptr };
  Ogre::Item* mItemRttPaint{ nullptr };
  Ogre::Item* mLightAxisItem{ nullptr };
  Ogre::SceneNode* mSceneNode{ nullptr };
  Ogre::SceneNode* mSceneNodeRttHoover{ nullptr };
  Ogre::SceneNode* mSceneNodeRttPaint{ nullptr };
  Ogre::SceneNode* mLightNode{ nullptr };
  Ogre::SceneNode* mSecondLightNode{ nullptr };
  Ogre::SceneNode* mLightAxisNode{ nullptr };
  Ogre::CompositorWorkspace* mWorkspace{ nullptr };
  Ogre::CompositorWorkspace* mWorkspaceRttHoover{ nullptr };
  Ogre::CompositorWorkspace* mWorkspaceRttPaint{ nullptr };
  Ogre::CompositorWorkspace* mWorkspaceRttSkyBox{ nullptr };
  bool mRotateCameraMode;
  bool mShiftDown;
  bool mMouseDown;
  Ogre::Light* mLight{ nullptr };
  Ogre::Light* mSecondLight{ nullptr };
  Ogre::TextureGpu* mCustomRenderTextureHoover{ nullptr };
  Ogre::TextureGpu* mCustomRenderTexturePaint{ nullptr };
  Ogre::TextureGpu* mRttHoover{ nullptr };
  Ogre::TextureGpu* mRttPaint{ nullptr };
  Ogre::ColourValue mHelpColour;
  Ogre::String mRenderTextureNameHoover;
  Ogre::String mRenderTextureNamePaint;
  int mLatestSubItemIndexHighlighted;
  Ogre::HlmsDatablock* mLatestSubItemDatablock;
  QSize mSize;
  Ogre::IdString mCurrentDatablockId;
  const size_t RTT_HOOVER_SIZE_X = 256;
  const size_t RTT_HOOVER_SIZE_Y = 144;  // 9/16 x RTT_HOOVER_SIZE_X
  const size_t RTT_PAINT_SIZE_X = 1820;
  const size_t RTT_PAINT_SIZE_Y = 1024;  // 9/16 x RTT_PAINT_SIZE_X
  QMap<int, QVector3D> mColourMap;
  QMap<size_t, Ogre::String> mSnapshotDatablocks;
  bool mHoover;
  bool mPaintMode;          // If true, painting is enabled
  bool mOffsetTextureMode;  // If true, moving textures (displacement) is enabled
  QMap<unsigned short, Ogre::String> helperIndicesAndNames;
  QVector<int> helperIndices;
  RenderwindowDockWidget* mRenderwindowDockWidget;
  Ogre::Vector2 helperVector2;
  Ogre::IdString mPbsDatablockBlockOffsetTexture;    // Use the IdString instead of a pointer, because the pointer may
                                                     // change
  Ogre::IdString mUnlitDatablockBlockOffsetTexture;  // Use the IdString instead of a pointer, because the pointer may
                                                     // change
  Ogre::PbsTextureTypes mPbsTextureTypeOffsetTexture;
  Ogre::uint8 mUnlitTextureTypeOffsetTexture;
  bool mResetMousePos;  // Used to indicate that diff between current mouse position and previous mouse position must be
                        // set to 0 to keep smoothness of mousemovent actions
  Ogre::String mHelperString;
  Ogre::IdString mHelperIdString;

  virtual void createCompositor();
  virtual void createCompositorRenderToTexture();
  void createSkyBoxCompositor();
  virtual void paintEvent(QPaintEvent* e);
  virtual void resizeEvent(QResizeEvent* e);
  virtual void keyPressEvent(QKeyEvent* ev);
  virtual void keyReleaseEvent(QKeyEvent* ev);
  virtual void mouseMoveEvent(QMouseEvent* e);
  // virtual void mouseWheelEvent(QWheelEvent* e);
  virtual void wheelEvent(QWheelEvent* e);
  virtual void mousePressEvent(QMouseEvent* e);
  virtual void mouseReleaseEvent(QMouseEvent* e);
  virtual void mouseDoubleClickEvent(QMouseEvent* event);
  virtual void enterEvent(QEvent* event);
  virtual void leaveEvent(QEvent* event);
  GL_CONTEXT getCurrentGlContext(void);
  void rotateLight(Ogre::Vector2 relativeMouseMove);
  const Ogre::ColourValue& calculateIndexToColour(int index);
  int calculateColourToIndex(const Ogre::ColourValue& colourValue);
  const Ogre::ColourValue& getColourAtRenderToTextureHoover(int x, int y);

  /* To highlight a subitem in on the screen, the following steps are performed
   * - Create a render texture and an additional workspace (createCompositorRenderToTexture)
   * - Create an additional Item (mItemRttHoover), based on the same mesh as the main Item (mItem) that is displayed on
   * the screen
   * - For each subItem in mItemRttHoover, a specific colour (unlit material) is assigned, with name 0, 1, 2, 3, ...
   * - The colour of is associated with the subItem index (using a colourmap (mColourMap))
   * - mItemRttHoover is only rendered on the render texture and not on the screen.
   *   0 This is done by manually updating the render-texture workspace in the updateOgre function
   *   0 The scenenodes to which mItem and mItemRttHoover are attached are made visible/invisible
   * - The colour in the render texture is picked (getColourAtRenderToTextureHoover); this is based on the mouse
   * position on the render window
   * - The colour is translated to the index of the subItem (calculateColourToIndex), using the colourmap
   * - The subItem (based on the calculated index) of the mItem is highlighted with a green material
   * (HIGHLIGHT_MATERIAL_NAME)
   */
  void highlightSubItem(Ogre::Vector2 mousePos);
  void setHighlightDatablockToSubItem(int index);
  void setColourToDatablockOfSubItem(int index, const Ogre::ColourValue& colour);
  const Ogre::ColourValue& getColourOfDatablockOfSubItem(int index);

  /* Functions used for painting
   */
  void setDefaultDatablockItemRttPaint(void);  // Set default datablock to the paint item
  void setUnlitDatablockRttPaint(void);        // Set the uv colour map datablock to the paint item
  const Ogre::Vector2& calculateColourToUv(const Ogre::ColourValue& col);  // Use a simple mapping algorithm to convert
                                                                           // a colourvalue to a uv
  const Ogre::ColourValue& getColourAtRenderToTexturePaint(int x, int y);  // Get the colour of the uv mapping texture
                                                                           // on a mouse position
  void doPaintSaveTextureGeneration(void);                                 // Save the current texture of a layer

  //            /* Apply the paint effect to the layers. This funtion returns a code. Depending on the code, a message
  //            can be displayed
  //             * doPaintLayer returns:
  //             * 0 - The painting action was succesful
  //             * 1 - No painting was done (don't display a message)
  //             * 2 - Painting is not possible, because the mousepointer did not hoover over the mesh (don't display a
  //             message)
  //             * 3 - There is no material assigned to the mesh
  //             * 4 - There is no texture type assigned in the paint layer
  //             * 5 - Painting is done on a material that is not the current material
  //             * The 'start'  argument indicates whether the painting starts
  //             */
  //            int doPaintLayer(int mouseX, int mouseY, bool start = false);

  Ogre::HlmsDatablock* getDatablockByNameStr(const Ogre::String& nameStr);
  //            PaintLayers* mPaintLayers; // Pointer to vector of PaintLayer objects
  //            int mLatestPaintResult; // This value is used to determines what the result of a painting action was

  /* Functions used moving the texture with the mouse (creating offset)
   */
  void doOffsetTexture(float relativeMouseX, float relativeMouseY);

  /* Clamp argument 'val' between [0..1]
   */
  float saturate(float val);

  /* Determine the index of a detail map
   */
  Ogre::uint8 getDetailMapIndexFromTextureTypeForScaleAndOffset(Ogre::PbsTextureTypes textureType);
};
}  // namespace tesseract_gui

#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_WIDGET_H
