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
#ifndef TESSERACT_QT_COMMON_JOINT_STATE_STANDARD_ITEM_H
#define TESSERACT_QT_COMMON_JOINT_STATE_STANDARD_ITEM_H

#include <QStandardItem>

namespace tesseract_common
{
class JointState;
}

namespace tesseract_gui
{
class JointStateStandardItem : public QStandardItem
{
public:
  explicit JointStateStandardItem(const tesseract_common::JointState& state);
  JointStateStandardItem(const QString& text, const tesseract_common::JointState& state);
  JointStateStandardItem(const QIcon& icon, const QString& text, const tesseract_common::JointState& state);
  int type() const override;

private:
  void ctor(const tesseract_common::JointState& state);
};
}  // namespace tesseract_gui
#endif  // TESSERACT_QT_COMMON_JOINT_STATE_STANDARD_ITEM_H
