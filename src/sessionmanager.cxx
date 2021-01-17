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

#include "sessionmanager.h"

#include <QDataStream>
#include <QFile>
#include <QReadLocker>
#include <QWriteLocker>
#include <QVariant>

constexpr int SESSION_VERSION = 0;

SessionManager::SessionManager()
{

}

SessionManager::~SessionManager()
{

}

SessionManager::LayerArray& SessionManager::layers()
{
  QReadLocker locker(&m_ioLock);
  return m_layers;
}

const SessionManager::LayerArray& SessionManager::layers() const
{
  QReadLocker locker(&m_ioLock);
  return m_layers;
}

void SessionManager::updateLayer(int index, const Layer& layer)
{
  m_ioLock.lockForWrite();
  if (index >= 0 && index < m_layers.size()) {
    m_layers[index] = layer;
  }
  m_ioLock.unlock();

  emit sync();
}


int SessionManager::layerCount() const
{
  QReadLocker locker(&m_ioLock);
  return m_layerCount;
}

void SessionManager::setLayerCount(int count)
{
  m_ioLock.lockForWrite();
  m_layerCount = count;
  m_ioLock.unlock();

  markSessionDirty();
  emit layerCountChanged(count);
}

void SessionManager::markSessionDirty()
{
  m_sessionIsDirty = true;
  emit sessionMarkedDirty(true);
}

bool SessionManager::isSessionDirty() const
{
  return m_sessionIsDirty;
}

int SessionManager::version()
{
  return SESSION_VERSION;
}

bool SessionManager::saveSession(const QString& fname)
{
  bool result = false;

  m_ioLock.lockForRead();
  QString outFname = fname.isEmpty() ? m_sessionFname : fname;
  if (!outFname.isEmpty()) {
    QFile file(outFname);
    if (file.open(QIODevice::WriteOnly)) {
      QDataStream out(&file);

      //Make header
      QVariantMap headerMap;
      headerMap["sessionVersion"]   = version();
      headerMap["layerVersion"]     = Layer::version();
      headerMap["materialVersion"]  = Material::version();

      //Write header
      out << headerMap;

      //Write layer information
      out << int(m_layers.size());
      for (const Layer& layer : m_layers) {
        out << layer;
      }

      out << m_layerCount;

      //Success!
      result = true;
    }
  }
  m_ioLock.unlock();

  if (result) {
    m_sessionFname = fname;

    //Reset dirty state
    m_sessionIsDirty = false;
    emit sessionMarkedDirty(false);
  }
  else {
    resetSession();
  }

  return result;
}

bool SessionManager::loadSession(const QString& fname)
{
  bool result = false;

  m_ioLock.lockForWrite();
  QFile file(fname);
  if (file.open(QIODevice::ReadOnly)) {
    QDataStream in(&file);

    //Read header
    QVariantMap headerMap;
    in >> headerMap;

    //TODO: Check header for version conflicts

    //Read layer information
    int count;
    in >> count;

    for (int i = 0; i < count; ++i) {
      in >> m_layers[i];
    }

    in >> m_layerCount;

    //Success!
    result = true;
  }
  m_ioLock.unlock();

  if (result) {
    m_sessionFname = fname;

    //Notify other components that the session has changed
    emit sessionChanged();

    //Reset dirty state
    m_sessionIsDirty = false;
    emit sessionMarkedDirty(false);
  }
  else {
    resetSession();
  }

  return result;
}

void SessionManager::resetSession()
{
  m_ioLock.lockForWrite();

  //Reset path to saved session
  m_sessionFname = "";

  //Dump layer array
  m_layers = LayerArray();
  m_layerCount = 2;

  //Reset dirty state
  m_sessionIsDirty = false;

  m_ioLock.unlock();

  emit  sessionMarkedDirty(false);

  //Notify other components that the session has changed
  emit sessionChanged();
}

QString SessionManager::sessionFilename() const
{
  return m_sessionFname;
}
