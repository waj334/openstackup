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

#include "netclassmodel.h"
#include "sessionmanager.h"

NetClassModel::NetClassModel(QObject* parent) :
  QAbstractTableModel(parent)
{
  onSync();

  connect(SessionManager::instance().get(), &SessionManager::sync,
    this, &NetClassModel::onSync, Qt::QueuedConnection);
  connect(SessionManager::instance().get(), &SessionManager::sessionChanged,
    this, &NetClassModel::onSync, Qt::QueuedConnection);
}

int NetClassModel::rowCount(const QModelIndex& parent) const
{
  QReadLocker locker(&m_ioLock);
  int count = 0;

  if (!parent.isValid()) {
    count = m_netClasses.count();
  }

  return count;
}

int NetClassModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QVariant NetClassModel::data(const QModelIndex& index, int role) const
{
  QReadLocker locker(&m_ioLock);
  QVariant data;

  if (index.isValid()) {
    auto netClass = m_netClasses[index.row()];

    if (role == Qt::DisplayRole) {
      switch (index.column()) {
      case 0:
        data = netClass.name();
        break;
      }
    }
    /*else if (role == Qt::EditRole) {
      switch (index.column()) {
      case 0:
        data = QString("%1 MHz")
          .arg(dk.m_frequency);
        break;
      case 1:
        data = QString::number(dk.m_dk, 'f');
        break;
      }
    }*/
  }

  return data;
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
  return flags;
}

void NetClassModel::onSync()
{
  // Lock for writing to prevent other threads from
  // changing the data during a render
  m_ioLock.lockForWrite();
  m_netClasses = SessionManager::instance()->netClasses();
  m_ioLock.unlock();

  m_ioLock.lockForRead();
  emit beginResetModel();
  emit endResetModel();
  m_ioLock.unlock();
}