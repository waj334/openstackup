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

#include <QDataStream>
#include <QList>
#include <QMetaType>
#include <QString>

class Net {
public:
  struct Wire {
    double m_length;
    int m_layer;
  };

  using WireList = QList<Wire>;

  Net();
  Net(const QString& name);
  ~Net();

  QString name() const;
  void setName(const QString& name);

  double length() const;

  WireList& wires();
  const WireList& wires() const;
  void updateWire(int index, const Wire& wire);

  static int version();
  QDataStream& write(QDataStream& stream) const;
  QDataStream& read(QDataStream& stream);

private:
  QString m_name;
  WireList m_wires;

  void readV0(QDataStream& stream);
};

Q_DECLARE_METATYPE(Net)

using NetList = QList<Net>;

inline QDataStream& operator<<(QDataStream& stream, const Net& net)
{
  return net.write(stream);
}

inline QDataStream& operator>>(QDataStream& stream, Net& net)
{
  return net.read(stream);
}