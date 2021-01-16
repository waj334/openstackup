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

#include <array>

class SessionManager : public Manager<SessionManager> {
  Q_OBJECT
public:
  using LayerArray = std::array<Layer, 31>;

  explicit SessionManager();
  ~SessionManager();

  LayerArray& layers();
  const LayerArray& layers() const;

  int layerCount() const;
  void setLayerCount(int count);

  void markSessionDirty();
  bool isSessionDirty() const;

  static int version();
  bool saveSession(const QString& fname = "");
  bool loadSession(const QString& fname);
  void resetSession();

  QString sessionFilename() const;

signals:
  void layerCountChanged(int);
  void sessionMarkedDirty(bool);
  void sessionChanged();

private:
  LayerArray m_layers;
  int m_layerCount = 2;
  bool m_sessionIsDirty = false;
  QString m_sessionFname;
};