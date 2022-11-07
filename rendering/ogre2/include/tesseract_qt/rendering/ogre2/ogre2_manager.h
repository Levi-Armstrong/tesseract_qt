#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_INTERFACE_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_INTERFACE_H

#include <QDebug>

#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>
#include <thread>
#include <unordered_map>
#include <tuple>

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <OgreMeshManager2.h>
#include <OgreMeshManager.h>
#include <OgreMesh2.h>
#include <OgreMesh.h>
#include <OgreItem.h>
#include <OgreHlms.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlit.h>
#include <OgreWindow.h>

#include <tesseract_qt/rendering/ogre2/common/graphics_api.h>

namespace tesseract_gui
{
/// Name of the DirectX 11 RenderSystem
static const Ogre::String OGRE2_RENDERSYSTEM_DIRECTX11 = "Direct3D11 Rendering Subsystem";
/// Name of the OpenGL RenderSystem
static const Ogre::String OGRE2_RENDERSYSTEM_OPENGL3PLUS = "OpenGL 3+ Rendering Subsystem";
/// Name of the Metal RenderSystem
static const Ogre::String OGRE2_RENDERSYSTEM_METAL = "Metal Rendering Subsystem";

// Necessary on Non windows platform :
#ifndef _WIN32
using HGLRC = void*;
#endif

class Ogre2Manager
{
public:
  using WidgetCreationCallbackTuple =
      std::tuple<Ogre::SceneManager*, Ogre::Camera*, Ogre::CompositorWorkspace*, Ogre::IdString>;

  /// Class constructor. this class is singleton
  Ogre2Manager(GraphicsAPI graphics_api);

  /// Class destructor
  ~Ogre2Manager();

  /// Return the instance of this singleton class
  static Ogre2Manager* instance();

  /// Helper function that return a V2 mesh from a V1 mesh. V1 mesh will be unallocated
  Ogre::MeshPtr loadFromV1Mesh(Ogre::v1::MeshPtr v1Mesh);

  /// Helper function that load the V1 mesh with the given name of a .mesh file known to the resource manager
  Ogre::MeshPtr loadFromV1Mesh(Ogre::String name);

  /// Get root object
  Ogre::Root* getRoot();

  /// Get the `index` scene
  Ogre::SceneManager* getScene(std::size_t index);

  /// Create a Scene and append it to the Scene List
  void createNewScene();

  /// Method called by each viewport widget when it is showed.
  WidgetCreationCallbackTuple WidgetCreatedCallback(Ogre::Window* virtualWindow, std::size_t sceneIndex = 0);

  /// Methdo called just before creating a window
  void willCreateWindowHint();

  /// Return the selected API. OpenGL or DirextX 11
  GraphicsAPI getGraphicsAPI();

  /// For Windows OpenGL only, return the context of the 1st window. Return null pointer if not relevent
  HGLRC getContext();

  /// Declare to the engine the location of the HLMS library
  void registerHlms();

  /// Write a message to Ogre's log
  static void logToOgre(std::string message);

  /// Set the AA level
  void setAALevel(uint8_t AA);

  /// Get the AA level
  uint8_t getAALevel();

  /// Set the default background color for viewports created after this one:
  void setDefaultBackgroundColor(const Ogre::ColourValue& c);

  /// Set the name of a compositor to use. You are responsible for actually loading that compositor resources
  void setCustomCompositor(const Ogre::String& name);

private:
  Ogre::String custom_compositor_;

  /// The AA level
  uint8_t aa_level_{ 8 };

  /// Singleton pointer
  static Ogre2Manager* self_;

  /// Name of the SL to use (for finding the correct HLMS to use)
  Ogre::String shading_language_;

  /// Smart pointer to the Ogre::Root, for automatic Ogre cleanup
  std::unique_ptr<Ogre::Root> root_{ nullptr };

  /// Number of threads the SMGRs will use to traverse and cull the geometry
  uint8_t threads_;

  /// List of the scenes to use
  std::vector<Ogre::SceneManager*> scenes_;

  /// Static counters for uniques names
  static size_t camera_counter_, workspace_counter_;

  /// The API we were requested to use.
  GraphicsAPI used_api_;

  /// Windows OpenGL context
  HGLRC gl_context_{ nullptr };

  /// Defautl background color for new viewports
  Ogre::ColourValue default_background_color_{ 0.2f, 0.4f, 0.6f };
};
}  // namespace tesseract_gui

#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_INTERFACE_H
