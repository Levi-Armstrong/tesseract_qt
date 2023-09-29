/**
 * @author Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @copyright Copyright (C) 2022 Levi Armstrong <levi.armstrong@gmail.com>
 *
 * @par License
 * GNU Lesser General Public License Version 3, 29 June 2007
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
<<<<<<< HEAD
#ifndef TESSERACT_QT_PLANNING_TASK_COMPOSER_CONTEXT_STANDARD_ITEM_H
#define TESSERACT_QT_PLANNING_TASK_COMPOSER_CONTEXT_STANDARD_ITEM_H
=======
<<<<<<< HEAD:command_language/include/tesseract_qt/command_language/models/cartesian_waypoint_standard_item.h
#ifndef TESSERACT_QT_COMMAND_LANGUAGE_CARTESIAN_WAYPOINT_STANDARD_ITEM_H
#define TESSERACT_QT_COMMAND_LANGUAGE_CARTESIAN_WAYPOINT_STANDARD_ITEM_H
=======
#ifndef TESSERACT_QT_PLANNING_TASK_COMPOSER_CONTEXT_STANDARD_ITEM_H
#define TESSERACT_QT_PLANNING_TASK_COMPOSER_CONTEXT_STANDARD_ITEM_H
>>>>>>> Update to use TaskComposerContext (#74):planning/include/tesseract_qt/planning/task_composer_context_standard_item.h
>>>>>>> Update to use TaskComposerContext (#74)

    #include < QStandardItem>

    namespace tesseract_planning
{
<<<<<<< HEAD
  class TaskComposerContext;
}

namespace tesseract_gui
{
=======
<<<<<<< HEAD:command_language/include/tesseract_qt/command_language/models/cartesian_waypoint_standard_item.h
  class CartesianWaypointPoly;
=======
  class TaskComposerContext;
>>>>>>> Update to use TaskComposerContext (#74):planning/include/tesseract_qt/planning/task_composer_context_standard_item.h
}
namespace tesseract_gui
{
<<<<<<< HEAD:command_language/include/tesseract_qt/command_language/models/cartesian_waypoint_standard_item.h
class CartesianWaypointStandardItem : public QStandardItem
{
public:
  explicit CartesianWaypointStandardItem(const tesseract_planning::CartesianWaypointPoly& cwp);
  CartesianWaypointStandardItem(const QString& text, const tesseract_planning::CartesianWaypointPoly& cwp);
  CartesianWaypointStandardItem(const QIcon& icon,
                                const QString& text,
                                const tesseract_planning::CartesianWaypointPoly& cwp);
  int type() const override;

private:
  void ctor(const tesseract_planning::CartesianWaypointPoly& cwp);
};
}  // namespace tesseract_gui
#endif  // TESSERACT_QT_COMMAND_LANGUAGE_CARTESIAN_WAYPOINT_STANDARD_ITEM_H
=======
>>>>>>> Update to use TaskComposerContext (#74)
class TaskComposerContextStandardItem : public QStandardItem
{
public:
  explicit TaskComposerContextStandardItem(const tesseract_planning::TaskComposerContext& context);
  TaskComposerContextStandardItem(const QString& text, const tesseract_planning::TaskComposerContext& context);
  TaskComposerContextStandardItem(const QIcon& icon,
                                  const QString& text,
                                  const tesseract_planning::TaskComposerContext& context);
  int type() const override;

private:
  void ctor(const tesseract_planning::TaskComposerContext& context);
};
}  // namespace tesseract_gui

#endif  // TESSERACT_QT_PLANNING_TASK_COMPOSER_CONTEXT_STANDARD_ITEM_H
<<<<<<< HEAD
=======
>>>>>>> Update to use TaskComposerContext (#74):planning/include/tesseract_qt/planning/task_composer_context_standard_item.h
>>>>>>> Update to use TaskComposerContext (#74)
