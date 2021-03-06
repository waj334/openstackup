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

#pragma once

#include "layer.h"
#include "manager.h"
#include "net.h"
#include "netclass.h"

#include <QReadWriteLock>

#include <array>

class SessionManager : public Manager<SessionManager> {
  Q_OBJECT
public:
  using LayerArray = std::array<Layer, 31>;

  explicit SessionManager();
  ~SessionManager();

  LayerArray& layers();
  const LayerArray& layers() const;
  void updateLayer(int index, const Layer& layer);

  int layerCount() const;
  void setLayerCount(int count);

  NetList& nets();
  const NetList& nets() const;
  void setNets(const NetList& nets);
  void updateNet(int index, const Net& net);
  const NetClass& netClass(const Net& net);

  NetClassList& netClasses();
  const NetClassList& netClasses() const;
  void setNetClasses(const NetClassList& netClasses);
  void updateNetClass(int index, const NetClass& netClass);
  bool netClassExists(const QString& name) const;
  const NetClass& defaultNetClass() const;

  void markSessionDirty();
  bool isSessionDirty() const;

  static int version();
  bool saveSession(const QString& fname = "");
  bool loadSession(const QString& fname);
  void resetSession();

  QString sessionFilename() const;

signals:
  void sync();
  void layerCountChanged(int);
  void sessionMarkedDirty(bool);
  void sessionChanged();

private:
  LayerArray m_layers;
  NetList m_nets;
  NetClassList m_netClasses;
  NetClass m_defaultNetClass;

  int m_layerCount = 2;
  bool m_sessionIsDirty = false;
  QString m_sessionFname;

  mutable QReadWriteLock m_ioLock;
};