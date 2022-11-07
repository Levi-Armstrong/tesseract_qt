/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_MATERIAL_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_MATERIAL_H

#include <memory>
#include <string>

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif
#include <Hlms/Pbs/OgreHlmsPbsPrerequisites.h>
#include <OgreGpuProgramParams.h>
#include <OgreMaterial.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <tesseract_qt/rendering/ogre2/common/color.h>
#include <tesseract_qt/rendering/ogre2/common/image.h>

namespace Ogre
{
class HlmsPbsDatablock;
class HlmsUnlitDatablock;
}  // namespace Ogre

namespace tesseract_gui
{
//
// forward declaration
class Ogre2MaterialPrivate;

/// \brief Ogre 2.x implementation of the material class
class Ogre2Material
{
  /// \brief Constructor
  Ogre2Material();

  /// \brief Destructor
public:
  virtual ~Ogre2Material();

  // Documentation inherited
public:
  void Destroy();

  // Documentation inherited
public:
  tesseract_common::Color Diffuse() const;

  // Documentation inherited
public:
  void SetDiffuse(const tesseract_common::Color& _color);

  // Documentation inherited
public:
  tesseract_common::Color Specular() const;

  // Documentation inherited
public:
  void SetSpecular(const tesseract_common::Color& _color);

  // Documentation inherited
public:
  tesseract_common::Color Emissive() const;

  // Documentation inherited
public:
  void SetEmissive(const tesseract_common::Color& _color);

  // Documentation inherited
public:
  void SetTransparency(const double _transparency);

  // Documentation inherited
public:
  void SetAlphaFromTexture(bool _enabled, double _alpha = 0.5, bool _twoSided = true);

  // Documentation inherited
public:
  float RenderOrder() const;

  // Documentation inherited
  // Review the official documentation to get more details about this
  // parameter, in particular mDepthBiasConstant
public:
  void SetRenderOrder(const float _renderOrder);

  // Documentation inherited
public:
  bool ReceiveShadows() const;

  // Documentation inherited
public:
  void SetReceiveShadows(const bool _receiveShadows);

  // Documentation inherited
public:
  bool HasTexture() const;

  // Documentation inherited
public:
  std::string Texture() const;

  // Documentation inherited
public:
  void SetTexture(const std::string& _texture, const std::shared_ptr<const tesseract_common::Image>& _img);

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> TextureData() const;

  // Documentation inherited
public:
  void ClearTexture();

  // Documentation inherited
public:
  bool HasNormalMap() const;

  // Documentation inherited
public:
  std::string NormalMap() const;

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> NormalMapData() const;

  // Documentation inherited
public:
  void SetNormalMap(const std::string& _normalMap, const std::shared_ptr<const tesseract_common::Image>& _img);

  // Documentation inherited
public:
  void ClearNormalMap();

  // Documentation inherited
public:
  bool HasRoughnessMap() const;

  // Documentation inherited
public:
  std::string RoughnessMap() const;

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> RoughnessMapData() const;

  // Documentation inherited
public:
  void SetRoughnessMap(const std::string& _roughnessMap, const std::shared_ptr<const tesseract_common::Image>& _img);

  // Documentation inherited
public:
  void ClearRoughnessMap();

  // Documentation inherited
public:
  bool HasMetalnessMap() const;

  // Documentation inherited
public:
  std::string MetalnessMap() const;

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> MetalnessMapData() const;

  // Documentation inherited
public:
  void SetMetalnessMap(const std::string& _metalnessMap, const std::shared_ptr<const tesseract_common::Image>& _img);

  // Documentation inherited
public:
  void ClearMetalnessMap();

  // Documentation inherited
public:
  bool HasEnvironmentMap() const;

  // Documentation inherited
public:
  std::string EnvironmentMap() const;

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> EnvironmentMapData() const;

  // Documentation inherited
public:
  void SetEnvironmentMap(const std::string& _environmentMap,
                         const std::shared_ptr<const tesseract_common::Image>& _img);

  // Documentation inherited
public:
  void ClearEnvironmentMap();

  // Documentation inherited
public:
  void SetRoughness(const float _roughness);

  // Documentation inherited
public:
  bool HasEmissiveMap() const;

  // Documentation inherited
public:
  std::string EmissiveMap() const;

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> EmissiveMapData() const;

  // Documentation inherited
public:
  void SetEmissiveMap(const std::string& _emissiveMap, const std::shared_ptr<const tesseract_common::Image>& _img);

  // Documentation inherited
public:
  void ClearEmissiveMap();

  // Documentation inherited
public:
  bool HasLightMap() const;

  // Documentation inherited
public:
  std::string LightMap() const;

  // Documentation inherited
public:
  unsigned int LightMapTexCoordSet() const;

  // Documentation inherited
public:
  void SetLightMap(const std::string& _lightMap,
                   const std::shared_ptr<const tesseract_common::Image>& _img,
                   unsigned int _uvSet = 0u);

  // Documentation inherited
public:
  std::shared_ptr<const tesseract_common::Image> LightMapData() const;

  // Documentation inherited
public:
  void ClearLightMap();

  // Documentation inherited
public:
  float Roughness() const;

  // Documentation inherited
public:
  void SetMetalness(const float _roughness);

  // Documentation inherited
public:
  float Metalness() const;

  /// \brief Return ogre low level material
  /// \return Ogre material pointer
public:
  virtual Ogre::MaterialPtr Material();

  /// \brief Return ogre Hlms material pbs datablock
  /// \return Ogre Hlms pbs datablock
public:
  virtual Ogre::HlmsPbsDatablock* Datablock() const;

  /// \brief Return ogre Hlms material unlit datablock
  /// \return Ogre Hlms unlit datablock
public:
  virtual Ogre::HlmsUnlitDatablock* UnlitDatablock();

  /// \brief Fill the input unlit datablock with current material
  /// properties from the pbs datablock
  /// \param[in] _datablock Unlit datablock to fill
public:
  virtual void FillUnlitDatablock(Ogre::HlmsUnlitDatablock* _datablock) const;

  // Documentation inherited.
  // \sa BaseMaterial::PreRender()
public:
  void PreRender();

  // Documentation inherited.
public:
  enum MaterialType Type() const;

  // Documentation inherited
public:
  bool DepthCheckEnabled() const;

  // Documentation inherited
public:
  void SetDepthCheckEnabled(bool _enabled);

  // Documentation inherited
public:
  bool DepthWriteEnabled() const;

  // Documentation inherited
public:
  void SetDepthWriteEnabled(bool _enabled);

  // Documentation inherited.
  // \sa Material::SetVertexShader(const std::string &)
public:
  void SetVertexShader(const std::string& _path);

  // Documentation inherited.
  // \sa Material::VertexShader() const
public:
  std::string VertexShader() const;

  // Documentation inherited.
  // \sa Material::VertexShaderParams()
public:
  std::shared_ptr<ShaderParams> VertexShaderParams();

  // Documentation inherited.
  // \sa Material::SetFragmentShader(const std::string &)
public:
  void SetFragmentShader(const std::string& _path);

  // Documentation inherited.
  // \sa Material::FragmentShader() const
public:
  std::string FragmentShader() const;

  // Documentation inherited.
  // \sa Material::FragmentShaderParams()
public:
  std::shared_ptr<ShaderParams> FragmentShaderParams();

protected:
  /// \brief Set the texture map for this material
  /// \param[in] _texture Name of the texture.
  /// \param[in] _type Type of texture, i.e. diffuse, normal, roughness,
  /// metalness
  virtual void SetTextureMapImpl(const std::string& _texture, Ogre::PbsTextureTypes _type);

  /// \brief Set the texture map for this material from an image loaded
  /// in memory
  /// \param[in] _texture Name of the texture.
  /// \param[in] _img tesseract_common::Image containing the texture data
  /// \param[in] _type Type of texture, i.e. diffuse, normal, roughness,
  /// metalness
  void SetTextureMapDataImpl(const std::string& _texture,
                             const std::shared_ptr<const tesseract_common::Image>& _img,
                             Ogre::PbsTextureTypes _type);

  /// \brief Get a pointer to the ogre texture by name
  /// \return Ogre texture
  virtual Ogre::TextureGpu* Texture(const std::string& _name);

  /// \brief Updates the material transparency in the engine,
  /// based on transparency and diffuse alpha values
  virtual void UpdateTransparency();

  // Documentation inherited.
  virtual void Init() override;

  /// \brief bind shader parameters that have changed
  void UpdateShaderParams();

  /// \brief Transfer params from gz-rendering type to ogre type
  /// \param[in] _params Gazebo Rendering params
  /// \param[out] _ogreParams ogre type for holding params
  void UpdateShaderParams(std::shared_ptr<const ShaderParams> _params, Ogre::GpuProgramParametersSharedPtr _ogreParams);

  /// \brief  Ogre material. Mainly used for render targets.
  Ogre::MaterialPtr ogreMaterial;

  /// \brief  Ogre data block containing all pbs material properties
  Ogre::HlmsPbsDatablock* ogreDatablock = nullptr;

  /// \brief  Ogre data block containing all unlit material properties
  Ogre::HlmsUnlitDatablock* ogreUnlitDatablock = nullptr;

  /// \brief  Ogre high level physical based shading object
  Ogre::HlmsPbs* ogreHlmsPbs = nullptr;

  /// \brief Name of the texture
  std::string textureName;

  /// \brief Name of the normal map
  std::string normalMapName;

  /// \brief Name of the roughness map
  std::string roughnessMapName;

  /// \brief Name of the metalness map
  std::string metalnessMapName;

  /// \brief Name of the environment map
  std::string environmentMapName;

  /// \brief Name of the emissive map
  std::string emissiveMapName;

  /// \brief Name of the light map
  std::string lightMapName;

  /// \brief Texture coorindate set used by the light map
  unsigned int lightMapUvSet = 0u;

  /// \brief Unique id assigned to ogre hlms datablock
  std::string ogreDatablockId;

private:
  /// \brief Pointer to private data class
  std::unique_ptr<Ogre2MaterialPrivate> dataPtr;
};
}  // namespace tesseract_gui
#endif
