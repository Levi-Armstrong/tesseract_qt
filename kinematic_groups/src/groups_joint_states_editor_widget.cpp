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
#include <tesseract_qt/kinematic_groups/groups_joint_states_editor_widget.h>
#include <tesseract_qt/kinematic_groups/group_joint_states_model.h>
#include <tesseract_qt/common/standard_item_type.h>
#include "ui_groups_joint_states_editor_widget.h"

#include <QStringListModel>

namespace tesseract_gui
{
struct GroupsJointStatesEditorWidgetImpl
{
  GroupJointsRetriever group_joints_retriever;
  GroupJointStatesModel* group_states_model;
  QStringListModel* group_names_model;
};

GroupsJointStatesEditorWidget::GroupsJointStatesEditorWidget(QWidget* parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::GroupsJointStatesEditorWidget>())
  , data_(std::make_unique<GroupsJointStatesEditorWidgetImpl>())
{
  ui_->setupUi(this);

  connect(ui_->groupNamesComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGroupNameChanged()));
  connect(ui_->addPushButton, SIGNAL(clicked()), this, SLOT(onAddJointState()));
  connect(ui_->removePushButton, SIGNAL(clicked()), this, SLOT(onRemoveJointState()));
  connect(ui_->jointSliderWidget,
          &tesseract_gui::JointStateSliderWidget::jointStateChanged,
          this,
          &tesseract_gui::GroupsJointStatesEditorWidget::jointStateChanged);

  onGroupNameChanged();
}

GroupsJointStatesEditorWidget::~GroupsJointStatesEditorWidget() = default;

void GroupsJointStatesEditorWidget::config(GroupJointsRetriever group_joints_retriever)
{
  data_->group_joints_retriever = group_joints_retriever;
  if (group_joints_retriever == nullptr)
    throw std::runtime_error("GroupsJointStatesEditorWidget, group joints retriever cannot be a nullptr!");
}

void GroupsJointStatesEditorWidget::setModels(tesseract_gui::GroupJointStatesModel* group_states_model,
                                              QStringListModel* group_names_model)
{
  data_->group_states_model = group_states_model;
  data_->group_names_model = group_names_model;
  ui_->treeView->setModel(data_->group_states_model);
  ui_->groupNamesComboBox->setModel(data_->group_names_model);
}

void GroupsJointStatesEditorWidget::onGroupNameChanged()
{
  if (data_->group_joints_retriever != nullptr)
    ui_->jointSliderWidget->setJoints(data_->group_joints_retriever(ui_->groupNamesComboBox->currentText()));
}

void GroupsJointStatesEditorWidget::onAddJointState()
{
  QString group_name = ui_->groupNamesComboBox->currentText();
  QString state_name = ui_->jointStateNameLineEdit->text();
  if (state_name.isEmpty())
    return;

  data_->group_states_model->addGroupJointState(group_name, state_name, ui_->jointSliderWidget->getJointState());
  ui_->jointStateNameLineEdit->clear();
}

void GroupsJointStatesEditorWidget::onRemoveJointState()
{
  QModelIndexList selection = ui_->treeView->selectionModel()->selectedIndexes();
  int row_cnt = selection.count();
  for (int i = row_cnt; i > 0; i--)
  {
    QStandardItem* item = data_->group_states_model->itemFromIndex(selection.at(i - 1));
    if (item->type() == static_cast<int>(StandardItemType::SRDF_GROUP_JOINT_STATE))
      data_->group_states_model->removeGroupJointState(item->parent()->text(), item->text());
  }
}

}  // namespace tesseract_gui
