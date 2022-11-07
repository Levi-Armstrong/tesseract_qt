#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_CONVERSIONS_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_CONVERSIONS_H

#include <string>
#include <OgreMaterial.h>

#include <tesseract_qt/common/entity_manager.h>
#include <tesseract_qt/common/entity_container.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/link.h>
#include <tesseract_geometry/geometry.h>
//#include <tesseract_rviz/types.h>
#include <Eigen/Core>

namespace Ogre
{
class SceneManager;
class SceneNode;
class Aabb;
class Item;
}  // namespace Ogre

namespace tesseract_gui
{
void toEigen(Eigen::Isometry3d& transform, const Ogre::Vector3& position, const Ogre::Quaternion& orientation);

void toOgre(Ogre::Vector3& position, Ogre::Quaternion& orientation, const Eigen::Isometry3d& transform);

bool isMeshWithColor(const std::string& file_path);

/** @brief Add tesseract resources to ogre */
void addOgreResourceLocation();

std::string getEnvNamespaceFromTopic(const std::string& topic);

std::vector<std::string> loadSceneGraph(Ogre::SceneManager& scene,
                                        Ogre::SceneNode& parent_node,
                                        tesseract_gui::EntityManager& entity_manager,
                                        const tesseract_scene_graph::SceneGraph& scene_graph,
                                        const std::string& prefix = "");

Ogre::SceneNode* loadLink(Ogre::SceneManager& scene,
                          tesseract_gui::EntityContainer& entity_container,
                          const tesseract_scene_graph::Link& link,
                          tesseract_scene_graph::Material::ConstPtr visual_material_override = nullptr,
                          tesseract_scene_graph::Material::ConstPtr collision_material_override = nullptr);

Ogre::SceneNode* loadLinkVisuals(Ogre::SceneManager& scene,
                                 tesseract_gui::EntityContainer& entity_container,
                                 const tesseract_scene_graph::Link& link,
                                 tesseract_scene_graph::Material::ConstPtr material_override = nullptr);

Ogre::SceneNode* loadLinkCollisions(Ogre::SceneManager& scene,
                                    tesseract_gui::EntityContainer& entity_container,
                                    const tesseract_scene_graph::Link& link,
                                    tesseract_scene_graph::Material::ConstPtr material_override = nullptr);

Ogre::SceneNode* loadLinkWireBox(Ogre::SceneManager& scene,
                                 tesseract_gui::EntityContainer& entity_container,
                                 const tesseract_scene_graph::Link& link,
                                 const Ogre::Aabb& aabb);

Ogre::SceneNode* loadLinkAxis(Ogre::SceneManager& scene,
                              tesseract_gui::EntityContainer& entity_container,
                              const tesseract_scene_graph::Link& link);

Ogre::SceneNode* loadLinkGeometry(Ogre::SceneManager& scene,
                                  tesseract_gui::EntityContainer& entity_container,
                                  const tesseract_geometry::Geometry& geometry,
                                  const Eigen::Vector3d& scale,
                                  const Eigen::Isometry3d& local_pose,
                                  const Ogre::MaterialPtr& material,
                                  bool is_visual);

Ogre::MaterialPtr loadLinkMaterial(Ogre::SceneManager& scene,
                                   const tesseract_scene_graph::Link& link,
                                   const std::string& material_name);

Ogre::MaterialPtr loadMaterial(const tesseract_scene_graph::Material& material);

Ogre::Item* createEntityForMeshData(Ogre::SceneManager& scene,
                                    tesseract_gui::EntityContainer& entity_container,
                                    const std::shared_ptr<const tesseract_common::VectorVector3d>& mesh_vertices,
                                    const std::shared_ptr<const Eigen::VectorXi>& mesh_faces);

// void setOctomapColor(double z_pos, double min_z, double max_z, double color_factor, rviz::PointCloud::Point* point);

// std::shared_ptr<rviz::PointCloud> createPointCloud(std::vector<rviz::PointCloud::Point>&& points,
//                                                   tesseract_gui::EntityContainer& entity_container,
//                                                   float size,
//                                                   tesseract_geometry::Octree::SubType subtype);

Ogre::Aabb getAABB(Ogre::SceneNode& scene_node, bool visible_only = true);

}  // namespace tesseract_gui
#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_CONVERSIONS_H
