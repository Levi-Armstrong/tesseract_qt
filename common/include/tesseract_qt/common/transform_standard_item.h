/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
 * @par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * @par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * @par
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef TESSERACT_QT_COMMON_TRANSFORM_STANDARD_ITEM_H
#define TESSERACT_QT_COMMON_TRANSFORM_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <Eigen/Geometry>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class PositionStandardItem;
class QuaternionStandardItem;
class TransformStandardItem : public QStandardItem
{
public:
  explicit TransformStandardItem(const Eigen::Isometry3d& transform);
  explicit TransformStandardItem(const QString& text, const Eigen::Isometry3d& transform);
  explicit TransformStandardItem(const QIcon& icon, const QString& text, const Eigen::Isometry3d& transform);
  int type() const override;

  void setTransform(const Eigen::Isometry3d& transform);

private:
  void ctor(const Eigen::Isometry3d& transform);
  PositionStandardItem* position_;
  QuaternionStandardItem* orientation_;
};
}  // namespace tesseract_gui

#endif  // TESSERACT_QT_COMMON_TRANSFORM_STANDARD_ITEM_H
