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

#include "netmodel.h"
#include "sessionmanager.h"

NetModel::NetModel(QObject* parent) :
  QAbstractTableModel(parent)
{
  connect(SessionManager::instance().get(), &SessionManager::sync,
    this, &NetModel::onSync, Qt::QueuedConnection);
  connect(SessionManager::instance().get(), &SessionManager::sessionChanged,
    this, &NetModel::onSync, Qt::QueuedConnection);
}

int NetModel::rowCount(const QModelIndex& parent) const
{
  int count = 0;

  if (!parent.isValid()) {
    count = SessionManager::instance()->nets().count();
  }

  return count;
}

int NetModel::columnCount(const QModelIndex& parent) const
{
  return 2;
}

bool NetModel::insertRows(int row, int count, const QModelIndex& parent)
{
  bool result = false;

  if (!parent.isValid() && row >= 0) {
    auto& nets = SessionManager::instance()->nets();

    beginInsertRows(parent, row, (row + count) - 1);

    for (int i = row; i < row + count; ++i) {
      Net net;
      net.setName("New net");

      nets.insert(i, net);
    }

    endInsertRows();
    result = true;
  }

  return result;
}

bool NetModel::removeRows(int row, int count, const QModelIndex& parent)
{
  bool result = false;

  if (!parent.isValid() && row >= 0) {
    auto& nets = SessionManager::instance()->nets();

    beginRemoveRows(parent, row, (row + count) - 1);

    for (int i = 0; i < count; ++i) {
      nets.removeAt(row);
    }

    endRemoveRows();
    result = true;
  }

  return result;
}

QVariant NetModel::data(const QModelIndex& index, int role) const
{
  QVariant data;

  if (index.isValid()) {
    auto net = SessionManager::instance()->nets()[index.row()];

    if (role == Qt::DisplayRole) {
      switch (index.column()) {
      case 0:
        data = net.name();
        break;
      case 1:
        data = QString("%1 mm")
          .arg(net.length(), 0, 'f');
        break;
      }
    }
    else if (role == Qt::EditRole) {
      switch (index.column()) {
      case 0:
        data = net.name();
        break;
      }
    }
  }

  return data;
}

bool NetModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  bool result = false;
  
  if (index.isValid()) {
    auto& net = SessionManager::instance()->nets()[index.row()];

    if (role == Qt::EditRole) {
      switch (index.column()) {
      case 0:
        net.setName(value.toString());
        break;
      }

      emit dataChanged(index, index, QVector<int>() << role);
      result = true;
    }
  }
  
  return result;
}

QVariant NetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant data;

  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      data = "Name";
      break;
    case 1:
      data = "Total Routed Distance";
      break;
    }
  }

  return data;
}

Qt::ItemFlags NetModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

  if (index.column() == 0) {
    flags |= Qt::ItemIsEditable;
  }

  return flags;
}

void NetModel::onSync()
{
  emit beginResetModel();
  emit endResetModel();
}