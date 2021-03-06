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

#include "permittivitymodel.h"

#include "material.h"

PermittivityModel::PermittivityModel(Material* material, QObject* parent) :
  QAbstractTableModel(parent),
  mp_material(material)
{
  Q_ASSERT(mp_material);
}

int PermittivityModel::rowCount(const QModelIndex& parent) const
{
  int count = 0;

  if (!parent.isValid()) {
    count = mp_material->permittivityList().count();
  }

  return count;
}

int PermittivityModel::columnCount(const QModelIndex& parent) const
{
  return 2;
}

QVariant PermittivityModel::data(const QModelIndex& index, int role) const
{
  QVariant data;

  if (index.isValid()) {
    auto dk = mp_material->permittivityList()[index.row()];

    if (role == Qt::DisplayRole) {
      switch (index.column()) {
      case 0:
        data = QString("%1 MHz")
          .arg(dk.m_frequency);
        break;
      case 1:
        data = QString::number(dk.m_dk, 'f');
        break;
      }
    }
    else if (role == Qt::EditRole) {
      switch (index.column()) {
      case 0:
        data = QString("%1 MHz")
          .arg(dk.m_frequency);
        break;
      case 1:
        data = QString::number(dk.m_dk, 'f');
        break;
      }
    }
  }

  return data;
}

bool PermittivityModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  bool result = false;

  if (role == Qt::EditRole && index.isValid()) {
    auto& dkList = mp_material->permittivityList();

    switch (index.column()) {
    case 0:
      dkList[index.row()].m_frequency = value.toUInt();
      break;
    case 1:
      dkList[index.row()].m_dk = value.toDouble();
      break;
    }

    emit dataChanged(index, index, QVector<int>() << Qt::EditRole);
    result = true;
  }

  return result;
}

QVariant PermittivityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant data;

  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      data = "Frequency";
      break;
    case 1:
      data = "Dk";
      break;
    }
  }

  return data;
}

Qt::ItemFlags PermittivityModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable 
    | Qt::ItemIsEditable;

  return flags;
}

bool PermittivityModel::insertRows(int row, int count, const QModelIndex& parent)
{
  bool result = false;

  if (!parent.isValid() && row >= 0) {
    auto& dkList = mp_material->permittivityList();

    beginInsertRows(parent, row, (row + count) - 1);

    for (int i = row; i < row + count; ++i) {
      Material::Permittivity dk;
      dk.m_dk = 0;
      dk.m_frequency = 0;

      dkList.insert(i, dk);
    }

    endInsertRows();
    result = true;
  }

  return result;
}

bool PermittivityModel::removeRows(int row, int count, const QModelIndex& parent)
{
  bool result = false;

  if (!parent.isValid() && row >= 0) {
    auto& dkList = mp_material->permittivityList();

    beginRemoveRows(parent, row, (row + count) - 1);

    for (int i = 0; i < count; ++i) {
      dkList.removeAt(row);
    }

    endRemoveRows();
    result = true;
  }

  return result;
}
