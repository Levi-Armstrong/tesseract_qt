#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_VIEWPORT_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_VIEWPORT_H

#include <QWidget>
#include <QPaintEvent>
#include <tesseract_qt/rendering/ogre2/ogre2_manager.h>
#include <tesseract_qt/rendering/ogre2/ogre2_cameraman.h>

namespace tesseract_gui
{
class Ogre2Viewport : public QWidget
{
  Q_OBJECT

public:
  /// Construct the viewport, give the index of the scene manager to use.
  /// If there's no created scene and the index is 0, will automatically create the 1st scene manager
  Ogre2Viewport(std::size_t smgr_index = 0, QWidget* parent = 0);

  ~Ogre2Viewport();

  /// Neutralize the paintEngine by returning nullptr
  QPaintEngine* paintEngine() const { return nullptr; }

  /// When the widget is repainted
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void moveEvent(QMoveEvent* event) override;

  /* How we handle keyboard and mouse events. */
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  // Manipulate the camera
  void setRotation(const Ogre::Vector3& rotation);
  void setPosition(const Ogre::Vector3& position);

  /** @brief Return camera used */
  Ogre::Camera* getCamera();

  /** @brief Return camera scene node */
  Ogre::SceneNode* getCameraNode();

  /** @brief Set position/orientation to default */
  void resetCamera();

  /** @brief Return the SceneManager used */
  Ogre::SceneManager* getSceneManager();

  /** @brief Enable sky */
  void setSkyEnabled(bool enabled);

  /** @brief Enable sky */
  void setGridEnabled(bool enabled);

private:
  /// Couner of windows
  static size_t window_number_;

  /// Index of the scene manager used
  size_t smgr_index_;

protected:
  bool system_initialized_{ false };

  /// The RenderWindow that ogre knows
  Ogre::Window* window_{ nullptr };

  /// The Camera
  std::unique_ptr<CameraMan> camera_manager_{ nullptr };
  Ogre::Camera* camera_{ nullptr };
  Ogre::SceneNode* camera_node_{ nullptr };
  bool skybox_enabled_{ false };

  /// The SMGR
  Ogre::SceneManager* scene_manager_{ nullptr };

  /// The Compositor workspace used
  Ogre::CompositorWorkspace* workspace_{ nullptr };

  /// The name of the compositor workspace definition
  Ogre::IdString workspace_def_name_;

  /// Mouse Event Data
  Ogre::Vector2 absolute_;
  Ogre::Vector2 relative_;
  bool rotate_camera_mode_{ true };
  bool reset_mouse_pos_{ true };  // Used to indicate that diff between current mouse position and previous mouse
                                  // position must be set to 0 to keep smoothness of mousemovent actions
  bool mouse_down_{ false };
};
}  // namespace tesseract_gui
#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_VIEWPORT_H
