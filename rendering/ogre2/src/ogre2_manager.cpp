#include <tesseract_qt/rendering/ogre2/ogre2_manager.h>
#include <tesseract_common/types.h>

namespace tesseract_gui
{
Ogre2Manager* Ogre2Manager::self_(nullptr);
size_t Ogre2Manager::camera_counter_(0);
size_t Ogre2Manager::workspace_counter_(0);

Ogre2Manager::Ogre2Manager(GraphicsAPI graphics_api)
{
  if (self_)
    throw std::runtime_error("Cannot instanciate 2 QtOgre21 objects. This is a singleton class.");

  threads_ = uint8_t(std::thread::hardware_concurrency());
  qDebug() << "Detected " << threads_ << " hardware threads";

  // Using half the threads for graphics only
  threads_ /= 2;

  qDebug() << "Scene Manager will have " << threads_ << "worker threads";
  qDebug() << "Init of Ogre for Qt5...\n";

  // Initialize Ogre Root
  root_ = std::make_unique<Ogre::Root>("", "ogre.cfg", "ogre.log");

  Ogre::RenderSystem* renderSystem{ nullptr };

  // Register the HLMS
  switch (graphics_api)
  {
    case GraphicsAPI::OPENGL:
    {
      qDebug() << "Rendering with OpenGL. Will use GLSL shaders";
      //#ifdef QT_DEBUG
      //        root->loadPlugin("./RenderSystem_GL3Plus_d");
      //#else
      root_->loadPlugin("/usr/lib/x86_64-linux-gnu/OGRE-2.2/OGRE/RenderSystem_GL3Plus");
      //#endif
      renderSystem = root_->getRenderSystemByName(OGRE2_RENDERSYSTEM_OPENGL3PLUS);
      renderSystem->setConfigOption("RTT Preferred Mode", "FBO");
      shading_language_ = "GLSL";
      break;
    }
    case GraphicsAPI::DIRECT3D11:
    {
      qDebug() << "Rendering with DirectX11. Will use HLSL shaders";
#ifdef QT_DEBUG
      root_->loadPlugin("./RenderSystem_DX11_d");
#else
      root->loadPlugin("./RenderSystem_DX11");
#endif
      renderSystem = root_->getRenderSystemByName(OGRE2_RENDERSYSTEM_DIRECTX11);
      shading_language_ = "HLSL";
      break;
    }
    case GraphicsAPI::METAL:
      /** @todo Implement */
      break;
    case GraphicsAPI::VULKAN:
      /** @todo Implement */
      break;
  }

  if (!renderSystem)
    throw std::runtime_error("Render System is NULLPTR");

  root_->setRenderSystem(renderSystem);
  //  renderSystem->setConfigOption("sRGB Gamma Conversion", "Yes");
  root_->initialise(false);

  // Store the singleton address
  self_ = this;
  used_api_ = graphics_api;

  //    registerHlms();
}

void Ogre2Manager::createNewScene()
{
  //    scenes.push_back(Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads,
  //    Ogre::INSTANCING_CULLING_THREADED));
  scenes_.push_back(Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads_));
}

Ogre2Manager::WidgetCreationCallbackTuple Ogre2Manager::WidgetCreatedCallback(Ogre::Window* virtualWindow,
                                                                              size_t sceneIndex)
{
  Ogre::SceneManager* smgr{ nullptr };
  if (scenes_.empty())
  {
    //        scenes.push_back(smgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads,
    //        Ogre::INSTANCING_CULLING_THREADED));
    scenes_.push_back(smgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads_));
    logToOgre("Just created a scene manager");
  }
  else
  {
    smgr = getScene(sceneIndex);
    logToOgre(std::string("Retreived the ") + std::to_string(sceneIndex) + "th scene manager");
  }

  Ogre::Camera* camera = smgr->createCamera("MyCamera" + std::to_string(camera_counter_++));

  auto compositorManager = root_->getCompositorManager2();
  Ogre::String workspaceNameStr;
  Ogre::IdString workspaceName;
  if (custom_compositor_.empty())
  {
    workspaceNameStr = "MyWorkspace" + std::to_string(workspace_counter_);
    workspaceName = workspaceNameStr;
    if (!compositorManager->hasWorkspaceDefinition(workspaceName))
      compositorManager->createBasicWorkspaceDef(workspaceNameStr, default_background_color_);
  }
  else
  {
    workspaceNameStr = custom_compositor_;
    workspaceName = custom_compositor_;
  }

  auto workspace = compositorManager->addWorkspace(
      smgr, virtualWindow->getTexture(), camera, workspaceName, true, int(workspace_counter_++));

  return std::tie<Ogre::SceneManager*, Ogre::Camera*, Ogre::CompositorWorkspace*, Ogre::IdString>(
      smgr, camera, workspace, workspaceName);
}

Ogre2Manager::~Ogre2Manager()
{
  // Forget the singleton address, because we're destructing
  self_ = nullptr;
}

Ogre2Manager* Ogre2Manager::instance() { return self_; }

Ogre::MeshPtr Ogre2Manager::loadFromV1Mesh(Ogre::String name)
{
  return loadFromV1Mesh(
      Ogre::v1::MeshManager::getSingleton().load(name,
                                                 Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                                                 Ogre::v1::HardwareBuffer::HBU_STATIC,
                                                 Ogre::v1::HardwareBuffer::HBU_STATIC));
}

Ogre::MeshPtr Ogre2Manager::loadFromV1Mesh(Ogre::v1::MeshPtr v1Mesh)
{
  Ogre::MeshPtr v2Mesh;
  v2Mesh = Ogre::MeshManager::getSingletonPtr()->createManual(v1Mesh->getName() + "v2",
                                                              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  v2Mesh->importV1(v1Mesh.get(), true, true, true);
  v1Mesh->unload();
  v1Mesh.setNull();

  return v2Mesh;
}

Ogre::SceneManager* Ogre2Manager::getScene(size_t index)
{
  if (index < scenes_.size())
    return scenes_[index];
  return nullptr;
}

void Ogre2Manager::registerHlms()
{
  const char* env = std::getenv("TESSERACT_QT_OGRE2_RESOURCE_PATH");

  tesseract_common::fs::path resourcePath = (env) ? std::string(env) : TESSERACT_QT_OGRE2_RESOURCE_PATH;
  tesseract_common::fs::path mediaPath(resourcePath);
  mediaPath.append("media");

  bool load_ignition{ false };
  if (!load_ignition)
  {
    mediaPath.append("ogre_next");
    tesseract_common::fs::path rootHlmsFolder{ mediaPath };

    tesseract_common::fs::path commonMaterialFolder{ rootHlmsFolder };
    commonMaterialFolder.append("2.0").append("scripts").append("materials").append("Common");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        commonMaterialFolder.c_str(), "FileSystem", "General");

    tesseract_common::fs::path commonGLSLMaterialFolder{ rootHlmsFolder };
    commonGLSLMaterialFolder.append("2.0").append("scripts").append("materials").append("Common").append("GLSL");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        commonGLSLMaterialFolder.c_str(), "FileSystem", "General");

    if (used_api_ == GraphicsAPI::METAL)
    {
      tesseract_common::fs::path commonMetalMaterialFolder{ rootHlmsFolder };
      commonMetalMaterialFolder.append("2.0").append("scripts").append("materials").append("Common").append("Metal");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          commonMetalMaterialFolder.c_str(), "FileSystem", "General");
    }

    // The following code is taken from the registerHlms() function in ogre2 samples framework
    if (rootHlmsFolder.empty())
      rootHlmsFolder = "./";

    // For retrieval of the paths to the different folders needed
    Ogre::String mainFolderPath;
    Ogre::StringVector libraryFoldersPaths;
    Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

    {
      Ogre::HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);

      tesseract_common::fs::path unlitMainFolderPath{ rootHlmsFolder };
      unlitMainFolderPath.append(mainFolderPath);

      std::vector<tesseract_common::fs::path> unlitLibraryFoldersPaths;
      for (const auto& p : libraryFoldersPaths)
        unlitLibraryFoldersPaths.push_back(tesseract_common::fs::path(rootHlmsFolder).append(p));

      Ogre::HlmsUnlit* hlmsUnlit{ nullptr };
      Ogre::Archive* archiveUnlit = archiveManager.load(unlitMainFolderPath.c_str(), "FileSystem", true);
      Ogre::ArchiveVec archiveUnlitLibraryFolders;

      for (const auto& p : unlitLibraryFoldersPaths)
      {
        Ogre::Archive* archiveLibrary = archiveManager.load(p.c_str(), "FileSystem", true);
        archiveUnlitLibraryFolders.push_back(archiveLibrary);
      }

      // Create and register the unlit Hlms
      hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
      Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);

      // disable writting debug output to disk
      //    hlmsUnlit->setDebugOutputPath(false, false);
      //    hlmsUnlit->setListener(hlmsUnlit);
    }

    {
      Ogre::HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);

      tesseract_common::fs::path pbsMainFolderPath{ rootHlmsFolder };
      pbsMainFolderPath.append(mainFolderPath);

      std::vector<tesseract_common::fs::path> pbsLibraryFoldersPaths;
      for (const auto& p : libraryFoldersPaths)
        pbsLibraryFoldersPaths.push_back(tesseract_common::fs::path(rootHlmsFolder).append(p));

      Ogre::HlmsPbs* hlmsPbs{ nullptr };
      Ogre::Archive* archivePbs = archiveManager.load(pbsMainFolderPath.c_str(), "FileSystem", true);

      // Get the library archive(s)
      Ogre::ArchiveVec archivePbsLibraryFolders;

      for (const auto& p : pbsLibraryFoldersPaths)
      {
        Ogre::Archive* archiveLibrary = archiveManager.load(p.c_str(), "FileSystem", true);
        archivePbsLibraryFolders.push_back(archiveLibrary);
      }

      // Create and register
      hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &archivePbsLibraryFolders);
      Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);

      // disable writting debug output to disk
      //    hlmsPbs->setDebugOutputPath(false, false);
      //    hlmsPbs->setListener(hlmsPbs);
    }
  }
  else
  {
    mediaPath.append("ignition");

    // register PbsMaterial resources
    tesseract_common::fs::path rootHlmsFolder{ mediaPath };
    tesseract_common::fs::path pbsCompositorFolder{ rootHlmsFolder };
    pbsCompositorFolder.append("2.0").append("scripts").append("Compositors");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        pbsCompositorFolder.c_str(), "FileSystem", "General");

    tesseract_common::fs::path commonMaterialFolder{ rootHlmsFolder };
    commonMaterialFolder.append("2.0").append("scripts").append("materials").append("Common");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        commonMaterialFolder.c_str(), "FileSystem", "General");

    tesseract_common::fs::path commonGLSLMaterialFolder{ rootHlmsFolder };
    commonGLSLMaterialFolder.append("2.0").append("scripts").append("materials").append("Common").append("GLSL");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        commonGLSLMaterialFolder.c_str(), "FileSystem", "General");

    tesseract_common::fs::path terraMaterialFolder{ rootHlmsFolder };
    terraMaterialFolder.append("2.0").append("scripts").append("materials").append("Terra");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        terraMaterialFolder.c_str(), "FileSystem", "General");

    tesseract_common::fs::path terraGLSLMaterialFolder{ rootHlmsFolder };
    terraGLSLMaterialFolder.append("2.0").append("scripts").append("materials").append("Terra").append("GLSL");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        terraGLSLMaterialFolder.c_str(), "FileSystem", "General");

    if (used_api_ == GraphicsAPI::METAL)
    {
      tesseract_common::fs::path commonMetalMaterialFolder{ rootHlmsFolder };
      commonMetalMaterialFolder.append("2.0").append("scripts").append("materials").append("Common").append("Metal");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          commonMetalMaterialFolder.c_str(), "FileSystem", "General");

      tesseract_common::fs::path terraMetalMaterialFolder{ rootHlmsFolder };
      terraMetalMaterialFolder.append("2.0").append("scripts").append("materials").append("Terra").append("Metal");
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          terraMetalMaterialFolder.c_str(), "FileSystem", "General");
    }

    // The following code is taken from the registerHlms() function in ogre2 samples framework
    if (rootHlmsFolder.empty())
      rootHlmsFolder = "./";

    // At this point rootHlmsFolder should be a valid path to the Hlms data folder

    // For retrieval of the paths to the different folders needed
    Ogre::String mainFolderPath;
    Ogre::StringVector libraryFoldersPaths;
    Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

    tesseract_common::fs::path customizationsArchiveLibraryFolder{ rootHlmsFolder };
    customizationsArchiveLibraryFolder.append("Hlms").append("Gz");
    Ogre::Archive* customizationsArchiveLibrary =
        archiveManager.load(customizationsArchiveLibraryFolder.c_str(), "FileSystem", true);

    {
      Ogre::HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);

      tesseract_common::fs::path unlitMainFolderPath{ rootHlmsFolder };
      unlitMainFolderPath.append(mainFolderPath);

      std::vector<tesseract_common::fs::path> unlitLibraryFoldersPaths;
      for (const auto& p : libraryFoldersPaths)
        unlitLibraryFoldersPaths.push_back(tesseract_common::fs::path(rootHlmsFolder).append(p));

      unlitLibraryFoldersPaths.push_back(
          tesseract_common::fs::path(rootHlmsFolder).append("Hlms").append("Gz").append("SolidColor"));
      unlitLibraryFoldersPaths.push_back(
          tesseract_common::fs::path(rootHlmsFolder).append("Hlms").append("Gz").append("SphericalClipMinDistance"));
      unlitLibraryFoldersPaths.push_back(
          tesseract_common::fs::path(rootHlmsFolder).append("Hlms").append("Gz").append("Pbs"));

      Ogre::HlmsUnlit* hlmsUnlit = 0;
      Ogre::Archive* archiveUnlit = archiveManager.load(unlitMainFolderPath.c_str(), "FileSystem", true);
      Ogre::ArchiveVec archiveUnlitLibraryFolders;

      for (const auto& p : unlitLibraryFoldersPaths)
      {
        Ogre::Archive* archiveLibrary = archiveManager.load(p.c_str(), "FileSystem", true);
        archiveUnlitLibraryFolders.push_back(archiveLibrary);
      }

      archiveUnlitLibraryFolders.push_back(customizationsArchiveLibrary);

      // Create and register the unlit Hlms
      hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
      Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);

      // disable writting debug output to disk
      //    hlmsUnlit->setDebugOutputPath(false, false);
      //    hlmsUnlit->setListener(hlmsUnlit);

      //    this->dataPtr->gzHlmsUnlit = hlmsUnlit;
    }

    {
      Ogre::HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);

      tesseract_common::fs::path pbsMainFolderPath{ rootHlmsFolder };
      pbsMainFolderPath.append(mainFolderPath);

      std::vector<tesseract_common::fs::path> pbsLibraryFoldersPaths;
      for (const auto& p : libraryFoldersPaths)
        pbsLibraryFoldersPaths.push_back(tesseract_common::fs::path(rootHlmsFolder).append(p));

      pbsLibraryFoldersPaths.push_back(
          tesseract_common::fs::path(rootHlmsFolder).append("Hlms").append("Gz").append("SolidColor"));
      pbsLibraryFoldersPaths.push_back(
          tesseract_common::fs::path(rootHlmsFolder).append("Hlms").append("Gz").append("SphericalClipMinDistance"));
      pbsLibraryFoldersPaths.push_back(
          tesseract_common::fs::path(rootHlmsFolder).append("Hlms").append("Gz").append("Pbs"));

      Ogre::HlmsPbs* hlmsPbs = 0;
      Ogre::Archive* archivePbs = archiveManager.load(pbsMainFolderPath.c_str(), "FileSystem", true);

      // Get the library archive(s)
      Ogre::ArchiveVec archivePbsLibraryFolders;

      {
        tesseract_common::fs::path pbs_path{ rootHlmsFolder };
        pbs_path.append("Hlms").append("Terra").append("GLSL").append("PbsTerraShadows");

        archivePbsLibraryFolders.push_back(archiveManager.load(pbs_path.c_str(), "FileSystem", true));
        //      this->dataPtr->hlmsPbsTerraShadows.reset(new Ogre::HlmsPbsTerraShadows());
      }

      for (const auto& p : pbsLibraryFoldersPaths)
      {
        Ogre::Archive* archiveLibrary = archiveManager.load(p.c_str(), "FileSystem", true);
        archivePbsLibraryFolders.push_back(archiveLibrary);
      }

      archivePbsLibraryFolders.push_back(customizationsArchiveLibrary);

      // Create and register
      hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &archivePbsLibraryFolders);
      Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);

      // disable writting debug output to disk
      //    hlmsPbs->setDebugOutputPath(false, false);
      //    hlmsPbs->setListener(hlmsPbs);

      //    dataPtr->gzHlmsPbs = hlmsPbs;
    }

    //  {
    //    Ogre::Ogre2GzHlmsTerra *hlmsTerra = 0;
    //    // Create & Register HlmsPbs
    //    // Do the same for HlmsPbs:
    //    Ogre::Ogre2GzHlmsTerra::GetDefaultPaths(mainFolderPath,
    //                                             libraryFoldersPaths);
    //    Ogre::Archive *archiveTerra = archiveManager.load(
    //        rootHlmsFolder + mainFolderPath, "FileSystem", true);

    //    // Get the library archive(s)
    //    Ogre::ArchiveVec archiveTerraLibraryFolders;
    //    libraryFolderPathIt = libraryFoldersPaths.begin();
    //    libraryFolderPathEn = libraryFoldersPaths.end();
    //    while (libraryFolderPathIt != libraryFolderPathEn)
    //    {
    //      Ogre::Archive *archiveLibrary =
    //          archiveManager.load(rootHlmsFolder + *libraryFolderPathIt,
    //          "FileSystem", true);
    //      archiveTerraLibraryFolders.push_back(archiveLibrary);
    //      ++libraryFolderPathIt;
    //    }

    //    // Create and register
    //    hlmsTerra = OGRE_NEW Ogre::Ogre2GzHlmsTerra(
    //      archiveTerra, &archiveTerraLibraryFolders,
    //      &this->dataPtr->sphericalClipMinDistance);
    //    Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsTerra);

    //    // disable writting debug output to disk
    //    hlmsTerra->setDebugOutputPath(false, false);
    //    hlmsTerra->setListener(hlmsTerra);

    //    this->dataPtr->terraWorkspaceListener.reset(
    //      new Ogre::TerraWorkspaceListener(hlmsTerra));

    //    this->dataPtr->gzHlmsTerra = hlmsTerra;
    //  }
  }
}

void Ogre2Manager::willCreateWindowHint()
{
  if (workspace_counter_ == 0)
    return;
    // Here one window has allready been created
#ifdef _WIN32
  if (used_api_ == RenderAPI::OpenGL)
    gl_context_ = wglGetCurrentContext();
  else
#endif
    gl_context_ = nullptr;
}

HGLRC Ogre2Manager::getContext() { return gl_context_; }

GraphicsAPI Ogre2Manager::getGraphicsAPI() { return used_api_; }

void Ogre2Manager::logToOgre(std::string message) { Ogre::LogManager::getSingleton().logMessage(message); }

void Ogre2Manager::setAALevel(uint8_t AA) { aa_level_ = AA; }

uint8_t Ogre2Manager::getAALevel() { return aa_level_; }

void Ogre2Manager::setDefaultBackgroundColor(const Ogre::ColourValue& c) { default_background_color_ = c; }

void Ogre2Manager::setCustomCompositor(const Ogre::String& name) { custom_compositor_ = name; }
}  // namespace tesseract_gui
