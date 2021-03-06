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

#include "netclassmodel.h"
#include "sessionmanager.h"

NetClassModel::NetClassModel(QObject* parent) :
  QAbstractTableModel(parent)
{
  connect(SessionManager::instance().get(), &SessionManager::sync,
    this, &NetClassModel::onSync, Qt::QueuedConnection);
  connect(SessionManager::instance().get(), &SessionManager::sessionChanged,
    this, &NetClassModel::onSync, Qt::QueuedConnection);
}

int NetClassModel::rowCount(const QModelIndex& parent) const
{
  int count = 0;

  if (!parent.isValid()) {
    count = SessionManager::instance()->netClasses().count();
  }

  return count;
}

int NetClassModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

bool NetClassModel::insertRows(int row, int count, const QModelIndex& parent)
{
  bool result = false;

  if (!parent.isValid() && row >= 0) {
    auto& nets = SessionManager::instance()->netClasses();

    beginInsertRows(parent, row, (row + count) - 1);

    for (int i = row; i < row + count; ++i) {
      NetClass netClass;
      netClass.setName("New Net Class");

      nets.insert(i, netClass);
    }

    endInsertRows();
    result = true;
  }

  return result;
}

bool NetClassModel::removeRows(int row, int count, const QModelIndex& parent)
{
  bool result = false;

  if (!parent.isValid() && row >= 0) {
    auto& nets = SessionManager::instance()->netClasses();

    beginRemoveRows(parent, row, (row + count) - 1);

    for (int i = 0; i < count; ++i) {
      nets.removeAt(row);
    }

    endRemoveRows();
    result = true;
  }

  return result;
}

QVariant NetClassModel::data(const QModelIndex& index, int role) const
{
  QVariant data;

  if (index.isValid()) {
    auto netClass = SessionManager::instance()->netClasses()[index.row()];

    if (role == Qt::DisplayRole) {
      switch (index.column()) {
      case 0:
        data = netClass.name();
        break;
      }
    }
    else if (role == Qt::EditRole) {
      switch (index.column()) {
      case 0:
        data = netClass.name();
        break;
      }
    }
  }

  return data;
}

bool NetClassModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  bool result = false;

  if (index.isValid()) {
    auto& netClass = SessionManager::instance()->netClasses()[index.row()];

    if (role == Qt::EditRole) {
      switch (index.column()) {
      case 0:
        netClass.setName(value.toString());
        break;
      }

      emit dataChanged(index, index, QVector<int>() << role);
      result = true;
    }
  }

  return result;
}

QVariant NetClassModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant data;

  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      data = "Name";
      break;
    }
  }

  return data;
}

  Qt::ItemFlags NetClassModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if (index.column() == 0) {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}

void NetClassModel::onSync()
{
  emit beginResetModel();
  emit endResetModel();
}