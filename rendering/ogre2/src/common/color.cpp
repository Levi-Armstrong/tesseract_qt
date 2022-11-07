/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <cmath>
#include <algorithm>

#include <tesseract_qt/rendering/ogre2/common/color.h>
#include <tesseract_common/utils.h>

namespace
{
// Use constexpr storage for the Color constants, to avoid the C++ static
// initialization order fiasco.
constexpr tesseract_common::Color gWhite = tesseract_common::Color(1, 1, 1, 1);
constexpr tesseract_common::Color gBlack = tesseract_common::Color(0, 0, 0, 1);
constexpr tesseract_common::Color gRed = tesseract_common::Color(1, 0, 0, 1);
constexpr tesseract_common::Color gGreen = tesseract_common::Color(0, 1, 0, 1);
constexpr tesseract_common::Color gBlue = tesseract_common::Color(0, 0, 1, 1);
constexpr tesseract_common::Color gYellow = tesseract_common::Color(1, 1, 0, 1);
constexpr tesseract_common::Color gMagenta = tesseract_common::Color(1, 0, 1, 1);
constexpr tesseract_common::Color gCyan = tesseract_common::Color(0, 1, 1, 1);

}  // namespace

namespace tesseract_common
{
const Color& Color::White = gWhite;
const Color& Color::Black = gBlack;
const Color& Color::Red = gRed;
const Color& Color::Green = gGreen;
const Color& Color::Blue = gBlue;
const Color& Color::Yellow = gYellow;
const Color& Color::Magenta = gMagenta;
const Color& Color::Cyan = gCyan;

//////////////////////////////////////////////////
void Color::reset()
{
  this->r = this->g = this->b = 0;
  this->a = 1;
}

//////////////////////////////////////////////////
void Color::set(const float _r, const float _g, const float _b, const float _a)
{
  this->r = _r;
  this->g = _g;
  this->b = _b;
  this->a = _a;

  this->Clamp();
}

//////////////////////////////////////////////////
void Color::setFromHSV(const float _h, const float _s, const float _v)
{
  int i;
  float f, p, q, t;

  float h = static_cast<float>(static_cast<int>(_h < 0 ? 0 : _h) % 360);

  if (almostEqualRelativeAndAbs(_s, 0.0f))
  {
    // acromatic (grey)
    this->r = this->g = this->b = _v;
    return;
  }

  // sector 0 - 5
  h /= 60;

  i = static_cast<int>(floor(h));

  f = h - i;

  p = _v * (1 - _s);
  q = _v * (1 - _s * f);
  t = _v * (1 - _s * (1 - f));

  switch (i)
  {
    case 0:
      this->r = _v;
      this->g = t;
      this->b = p;
      break;
    case 1:
      this->r = q;
      this->g = _v;
      this->b = p;
      break;
    case 2:
      this->r = p;
      this->g = _v;
      this->b = t;
      break;
    case 3:
      this->r = p;
      this->g = q;
      this->b = _v;
      break;
    case 4:
      this->r = t;
      this->g = p;
      this->b = _v;
      break;
    case 5:
    default:
      this->r = _v;
      this->g = p;
      this->b = q;
      break;
  }

  this->Clamp();
}

//////////////////////////////////////////////////
Eigen::Vector3f Color::getHSV() const
{
  Eigen::Vector3f hsv;

  float min = std::min(this->r, std::min(this->g, this->b));
  float max = std::max(this->r, std::max(this->g, this->b));
  float delta = max - min;

  hsv.y() = delta / max;
  hsv.z() = max;

  if (almostEqualRelativeAndAbs(delta, 0.0f))
  {
    hsv.x() = 0.0;
    hsv.y() = 0.0;
  }
  else if (almostEqualRelativeAndAbs(this->r, min))
    hsv.x() = 3 - ((this->g - this->b) / delta);
  else if (almostEqualRelativeAndAbs(this->g, min))
    hsv.x() = 5 - ((this->b - this->r) / delta);
  else
    hsv.x() = 1 - ((this->r - this->g) / delta);

  hsv.x() *= 60.0;
  return hsv;
}

//////////////////////////////////////////////////
Eigen::Vector3f Color::getYUV() const
{
  Eigen::Vector3f yuv;

  yuv.x() = 0.299f * this->r + 0.587f * this->g + 0.114f * this->b;
  yuv.y() = -0.1679f * this->r - 0.332f * this->g + 0.5f * this->b + 0.5f;
  yuv.z() = 0.5f * this->r - 0.4189f * this->g - 0.08105f * this->b + 0.5f;

  yuv.x() = yuv.x() < 0 ? 0 : yuv.x();
  yuv.x() = yuv.x() > 255 ? 255.0f : yuv.x();

  yuv.y() = yuv.y() < 0 ? 0 : yuv.y();
  yuv.y() = yuv.y() > 255 ? 255.0f : yuv.y();

  yuv.z() = yuv.z() < 0 ? 0 : yuv.z();
  yuv.z() = yuv.z() > 255 ? 255.0f : yuv.z();

  return yuv;
}

//////////////////////////////////////////////////
void Color::setFromYUV(const float _y, const float _u, const float _v)
{
  this->r = _y + 1.140f * _v;
  this->g = _y - 0.395f * _u - 0.581f * _v;
  this->b = _y + 2.032f * _u;
  this->Clamp();
}

//////////////////////////////////////////////////
float Color::operator[](const unsigned int _index) { return (*static_cast<const Color*>(this))[_index]; }

//////////////////////////////////////////////////
float Color::operator[](const unsigned int _index) const
{
  switch (_index)
  {
    case 0:
      return this->r;
    case 1:
      return this->g;
    case 2:
      return this->b;
    case 3:
      return this->a;
    default:
      break;
  }

  return std::numeric_limits<float>::quiet_NaN();
}

//////////////////////////////////////////////////
Color::RGBA Color::asRGBA() const
{
  uint8_t val8;
  unsigned int val32;

  // Convert to 32bit pattern
  // (RGBA = 8888)

  val8 = static_cast<uint8_t>(this->r * 255);
  val32 = val8 << 24;

  val8 = static_cast<uint8_t>(this->g * 255);
  val32 += val8 << 16;

  val8 = static_cast<uint8_t>(this->b * 255);
  val32 += val8 << 8;

  val8 = static_cast<uint8_t>(this->a * 255);
  val32 += val8;

  return val32;
}

//////////////////////////////////////////////////
Color::BGRA Color::asBGRA() const
{
  uint8_t val8;
  unsigned int val32 = 0;

  // Convert to 32bit pattern
  // (BGRA = 8888)

  val8 = static_cast<uint8_t>(this->b * 255);
  val32 = val8 << 24;

  val8 = static_cast<uint8_t>(this->g * 255);
  val32 += val8 << 16;

  val8 = static_cast<uint8_t>(this->r * 255);
  val32 += val8 << 8;

  val8 = static_cast<uint8_t>(this->a * 255);
  val32 += val8;

  return val32;
}

//////////////////////////////////////////////////
Color::ARGB Color::asARGB() const
{
  uint8_t val8;
  unsigned int val32 = 0;

  // Convert to 32bit pattern
  // (ARGB = 8888)

  val8 = static_cast<uint8_t>(this->a * 255);
  val32 = val8 << 24;

  val8 = static_cast<uint8_t>(this->r * 255);
  val32 += val8 << 16;

  val8 = static_cast<uint8_t>(this->g * 255);
  val32 += val8 << 8;

  val8 = static_cast<uint8_t>(this->b * 255);
  val32 += val8;

  return val32;
}

//////////////////////////////////////////////////
Color::ABGR Color::asABGR() const
{
  uint8_t val8;
  unsigned int val32 = 0;

  // Convert to 32bit pattern
  // (ABGR = 8888)

  val8 = static_cast<uint8_t>(this->a * 255);
  val32 = val8 << 24;

  val8 = static_cast<uint8_t>(this->b * 255);
  val32 += val8 << 16;

  val8 = static_cast<uint8_t>(this->g * 255);
  val32 += val8 << 8;

  val8 = static_cast<uint8_t>(this->r * 255);
  val32 += val8;

  return val32;
}

//////////////////////////////////////////////////
void Color::setFromRGBA(const Color::RGBA _v)
{
  unsigned int val32 = _v;

  // Convert from 32bit pattern
  // (RGBA = 8888)

  this->r = ((val32 >> 24) & 0xFF) / 255.0f;
  this->g = ((val32 >> 16) & 0xFF) / 255.0f;
  this->b = ((val32 >> 8) & 0xFF) / 255.0f;
  this->a = (val32 & 0xFF) / 255.0f;
}

//////////////////////////////////////////////////
void Color::setFromBGRA(const Color::BGRA _v)
{
  unsigned int val32 = _v;

  // Convert from 32bit pattern
  // (BGRA = 8888)

  this->b = ((val32 >> 24) & 0xFF) / 255.0f;
  this->g = ((val32 >> 16) & 0xFF) / 255.0f;
  this->r = ((val32 >> 8) & 0xFF) / 255.0f;
  this->a = (val32 & 0xFF) / 255.0f;
}

//////////////////////////////////////////////////
void Color::setFromARGB(const Color::ARGB _v)
{
  unsigned int val32 = _v;

  // Convert from 32bit pattern
  // (ARGB = 8888)

  this->a = ((val32 >> 24) & 0xFF) / 255.0f;
  this->r = ((val32 >> 16) & 0xFF) / 255.0f;
  this->g = ((val32 >> 8) & 0xFF) / 255.0f;
  this->b = (val32 & 0xFF) / 255.0f;
}

//////////////////////////////////////////////////
void Color::setFromABGR(const Color::ABGR _v)
{
  unsigned int val32 = _v;

  // Convert from 32bit pattern
  // (ABGR = 8888)

  this->a = ((val32 >> 24) & 0xFF) / 255.0f;
  this->b = ((val32 >> 16) & 0xFF) / 255.0f;
  this->g = ((val32 >> 8) & 0xFF) / 255.0f;
  this->r = (val32 & 0xFF) / 255.0f;
}

//////////////////////////////////////////////////
Color Color::operator+(const Color& _pt) const
{
  return Color(this->r + _pt.r, this->g + _pt.g, this->b + _pt.b, this->a + _pt.a);
}

//////////////////////////////////////////////////
Color Color::operator+(const float& _v) const { return Color(this->r + _v, this->g + _v, this->b + _v, this->a + _v); }

//////////////////////////////////////////////////
const Color& Color::operator+=(const Color& _pt)
{
  this->r += _pt.r;
  this->g += _pt.g;
  this->b += _pt.b;
  this->a += _pt.a;

  this->Clamp();

  return *this;
}

//////////////////////////////////////////////////
Color Color::operator-(const Color& _pt) const
{
  return Color(this->r - _pt.r, this->g - _pt.g, this->b - _pt.b, this->a - _pt.a);
}

//////////////////////////////////////////////////
Color Color::operator-(const float& _v) const { return Color(this->r - _v, this->g - _v, this->b - _v, this->a - _v); }

//////////////////////////////////////////////////
const Color& Color::operator-=(const Color& _pt)
{
  this->r -= _pt.r;
  this->g -= _pt.g;
  this->b -= _pt.b;
  this->a -= _pt.a;

  this->Clamp();

  return *this;
}

//////////////////////////////////////////////////
const Color Color::operator/(const float& _i) const
{
  return Color(this->r / _i, this->g / _i, this->b / _i, this->a / _i);
}

//////////////////////////////////////////////////
const Color Color::operator/(const Color& _pt) const
{
  return Color(this->r / _pt.r, this->g / _pt.g, this->b / _pt.b, this->a / _pt.a);
}

//////////////////////////////////////////////////
const Color& Color::operator/=(const Color& _pt)
{
  this->r /= _pt.r;
  this->g /= _pt.g;
  this->b /= _pt.b;
  this->a /= _pt.a;

  this->Clamp();

  return *this;
}

//////////////////////////////////////////////////
const Color Color::operator*(const float& _i) const
{
  return Color(this->r * _i, this->g * _i, this->b * _i, this->a * _i);
}

//////////////////////////////////////////////////
const Color Color::operator*(const Color& _pt) const
{
  return Color(this->r * _pt.r, this->g * _pt.g, this->b * _pt.b, this->a * _pt.a);
}

//////////////////////////////////////////////////
const Color& Color::operator*=(const Color& _pt)
{
  this->r *= _pt.r;
  this->g *= _pt.g;
  this->b *= _pt.b;
  this->a *= _pt.a;

  this->Clamp();

  return *this;
}

//////////////////////////////////////////////////
bool Color::operator==(const Color& _pt) const
{
  return almostEqualRelativeAndAbs(this->r, _pt.r) && almostEqualRelativeAndAbs(this->g, _pt.g) &&
         almostEqualRelativeAndAbs(this->b, _pt.b) && almostEqualRelativeAndAbs(this->a, _pt.a);
}

//////////////////////////////////////////////////
bool Color::operator!=(const Color& _pt) const { return !(*this == _pt); }

//////////////////////////////////////////////////
float Color::getR() const { return this->r; }

//////////////////////////////////////////////////
float Color::getG() const { return this->g; }

//////////////////////////////////////////////////
float Color::getB() const { return this->b; }

//////////////////////////////////////////////////
float Color::getA() const { return this->a; }

//////////////////////////////////////////////////
float& Color::getR() { return this->r; }

//////////////////////////////////////////////////
float& Color::getG() { return this->g; }

//////////////////////////////////////////////////
float& Color::getB() { return this->b; }

//////////////////////////////////////////////////
float& Color::getA() { return this->a; }

//////////////////////////////////////////////////
void Color::setR(const float _r) { this->r = _r; }

//////////////////////////////////////////////////
void Color::setG(const float _g) { this->g = _g; }

//////////////////////////////////////////////////
void Color::setB(const float _b) { this->b = _b; }

//////////////////////////////////////////////////
void Color::setA(const float _a) { this->a = _a; }
}  // namespace tesseract_common
