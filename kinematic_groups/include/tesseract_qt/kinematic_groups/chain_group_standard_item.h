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
#ifndef TESSERACT_QT_KINEMATICS_GROUP_CHAIN_GROUP_STANDARD_ITEM_H
#define TESSERACT_QT_KINEMATICS_GROUP_CHAIN_GROUP_STANDARD_ITEM_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <QStandardItem>

namespace tesseract_gui
{
class ChainGroupStandardItem : public QStandardItem
{
public:
  ChainGroupStandardItem(tesseract_srdf::ChainGroup group);
  explicit ChainGroupStandardItem(const QString& text, tesseract_srdf::ChainGroup group);
  ChainGroupStandardItem(const QIcon& icon, const QString& text, tesseract_srdf::ChainGroup group);
  int type() const override;

  tesseract_srdf::ChainGroup group;

private:
  void ctor();
};
}  // namespace tesseract_gui

#endif  // TESSERACT_QT_KINEMATICS_GROUP_CHAIN_GROUP_STANDARD_ITEM_H
