/*
 * This file is part of the Open Stackup distribution (https://github.com/waj334/openstackup).
 * Copyright (c) 2021 Justin A. Wilson.
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

#include "permittivitydelegate.h"

#include <QLineEdit>
#include <QRegularExpression>

PermittivityDelegate::PermittivityDelegate(QObject* parent) :
  QStyledItemDelegate(parent)
{

}

QWidget* PermittivityDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
  const QModelIndex& index) const
{
  QWidget* widget = new QLineEdit(parent);
  return widget;
}

void PermittivityDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QLineEdit* le = qobject_cast<QLineEdit*>(editor);

  if (index.column() == 0) {
    quint32 value = index.data(Qt::EditRole).toUInt();

    QString str = QString("%1 MHz")
      .arg(value);

    le->setText(str);
  }
  else {
    double value = index.data(Qt::EditRole).toDouble();
    le->setText(QString::number(value, 'f'));
  }
}

void PermittivityDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
  const QModelIndex& index) const
{
  QLineEdit* le = qobject_cast<QLineEdit*>(editor);

  if (index.column() == 0) {
    // Convert to MHz
    QRegularExpression regex("(\\d*\\.?\\d+)\\s?(([mMgG])(\\w+))?$");
    auto matches = regex.match(le->text());

    if (matches.hasMatch()) {
      double value = matches.captured(1).toDouble();

      if (matches.lastCapturedIndex() > 1) {
        QString prefix = matches.captured(3).toLower();

        if (prefix == "g") {
          value *= 1000;
        }
      } //else assume unit is MHz

      //Set value
      model->setData(index, value, Qt::EditRole);
    }
  }
  else {
    double value = le->text().toDouble();
    model->setData(index, value, Qt::EditRole);
  }
}