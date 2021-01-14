/*
 * This file is part of the Open Stackup distribution (https://github.com/waj334/openstackup).
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "enums.h"
#include "material.h"
#include "materialmanager.h"
#include "stackupdelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QRegularExpression>

StackupDelegate::StackupDelegate(QObject* parent) :
  QStyledItemDelegate(parent)
{

}

QWidget* StackupDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
  const QModelIndex& index) const
{
  QWidget* widget = nullptr;

  if (index.column() == 1) {
    //Create material type selector
    QComboBox* cb = new QComboBox(parent);

    cb->addItem("Copper", QVariant::fromValue<MaterialClass>(MaterialClass::COPPER));
    cb->addItem("Core", QVariant::fromValue<MaterialClass>(MaterialClass::CORE));
    cb->addItem("Prepreg", QVariant::fromValue<MaterialClass>(MaterialClass::PREPREG));

    widget = cb;
  }
  else if (index.column() == 2) {
    QComboBox* cb = new QComboBox(parent);
    QList<Material> mats;
    MaterialManager::instance()->getAll(mats);
    
    //Get selected material type
    auto type = index.siblingAtColumn(1).data(Qt::EditRole).value<MaterialClass>();

    //Add materials matching 'type' to combo box
    for (const auto& mat : mats) {
      if (type == mat.materialClass()) {
        cb->addItem(mat.name(), QVariant::fromValue<Material>(mat));
      }
    }
  }
  else if (index.column() == 3) {
    widget = new QLineEdit(parent);
  }
  else {
    widget = QStyledItemDelegate::createEditor(parent, option, index);
  }

  return widget;
}

void StackupDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  if (index.column() == 1) {
    auto type = index.data(Qt::EditRole).value<MaterialClass>();
    QComboBox* cb = qobject_cast<QComboBox*>(editor);

    //Find item representing type
    int index = -1;
    for (int i = 0; i < cb->count(); ++i) {
      if (cb->itemData(i, Qt::EditRole).value<MaterialClass>() == type) {
        index = i;
        break;
      }
    }

    if (index >= 0) {
      cb->setCurrentIndex(index);
    }
  }
  else if (index.column() == 2) {
    auto mat = index.data(Qt::EditRole).value<Material>();
    QComboBox* cb = qobject_cast<QComboBox*>(editor);

    //Find item representing type
    int index = -1;
    for (int i = 0; i < cb->count(); ++i) {
      if (cb->itemData(i, Qt::EditRole).value<Material>().name() == mat.name()) {
        index = i;
        break;
      }
    }

    if (index >= 0) {
      cb->setCurrentIndex(index);
    }
  }
  else if (index.column() == 3) {
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    double value = index.data(Qt::EditRole).toDouble();

    QString str = QString("%1 mm")
      .arg(value, 0, 'f', 9);

    le->setText(str);
  }
  else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void StackupDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
  const QModelIndex& index) const
{
  if (index.column() == 1 || index.column() == 2) {
    QComboBox* cb = qobject_cast<QComboBox*>(editor);
    model->setData(index, cb->currentData(), Qt::EditRole);
  }
  else if (index.column() == 3) {
    //Handle units for thickness field. Convert to mm
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);

    QRegularExpression regex("(\\d*\\.?\\d+)\\s?(([munp])(\\w))?$");
    auto matches = regex.match(le->text());

    if (matches.hasMatch()) {
      double value = matches.captured(1).toDouble();

      if (matches.lastCapturedIndex() > 1) {
        QString prefix = matches.captured(3);

        if (prefix == "p") {
          value *= 1e-9;
        }
        else if (prefix == "n") {
          value *= 1e-6;
        }
        else if (prefix == "u") {
          value *= 1e-3;
        }
      } //else assume unit is mm

      //Set value
      model->setData(index, value, Qt::EditRole);
    }
  }
  else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}