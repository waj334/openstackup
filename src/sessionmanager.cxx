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

NetList& SessionManager::nets()
{
  QReadLocker locker(&m_ioLock);
  return m_nets;
}

const NetList& SessionManager::nets() const
{
  QReadLocker locker(&m_ioLock);
  return m_nets;
}

void SessionManager::setNets(const NetList& nets)
{
  m_ioLock.lockForWrite();
  m_nets = nets;
  m_ioLock.unlock();

  emit sync();
}

void SessionManager::updateNet(int index, const Net& net)
{
  m_ioLock.lockForWrite();
  if (index >= 0 && index < m_nets.size()) {
    m_nets[index] = net;
  }
  m_ioLock.unlock();

  emit sync();
}

const NetClass& SessionManager::netClass(const Net& net)
{
  for (const auto& netClass : m_netClasses) {
    if (netClass.nets().contains(net.name())) {
      return netClass;
    }
  }

  return m_defaultNetClass;
}

NetClassList& SessionManager::netClasses()
{
  QReadLocker locker(&m_ioLock);
  return m_netClasses;
}

const NetClassList& SessionManager::netClasses() const
{
  QReadLocker locker(&m_ioLock);
  return m_netClasses;
}

void SessionManager::setNetClasses(const NetClassList& netClasses)
{
  m_ioLock.lockForWrite();
  m_netClasses = netClasses;
  m_ioLock.unlock();

  emit sync();
}

void SessionManager::updateNetClass(int index, const NetClass& netClass)
{
  m_ioLock.lockForWrite();
  if (index >= 0 && index < m_netClasses.size()) {
    m_netClasses[index] = netClass;
  }
  m_ioLock.unlock();

  emit sync();
}

bool SessionManager::netClassExists(const QString& name) const
{
  bool result = false;
  
  for (const auto& netClass : m_netClasses) {
    if (netClass.name() == name) {
      result = true;
      break;
    }
  }
  
  return result;
}

const NetClass& SessionManager::defaultNetClass() const
{
  return m_defaultNetClass;
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
      headerMap["netVersion"]       = Net::version();
      headerMap["netClassVersion"] = NetClass::version();

      //Write header
      out << headerMap;

      //Write layer information
      out << int(m_layers.size()); //cast to int
      for (const Layer& layer : m_layers) {
        out << layer;
      }

      //Write net information
      out << m_nets.count();
      for (const Net& net : m_nets) {
        out << net;
      }

      //Write net class information
      out << m_netClasses.count();
      for (const NetClass& netClass : m_netClasses) {
        out << netClass;
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

    //Read net information
    m_nets.clear();

    in >> count;
    for (int i = 0; i < count; ++i) {
      Net net;
      in >> net;

      m_nets << net;
    }

    //Read net class information
    in >> count;
    for (int i = 0; i < count; ++i) {
      NetClass netClass;
      in >> netClass;

      m_netClasses << netClass;
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

  //Dump nets
  m_nets.clear();

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
