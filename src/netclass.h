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
#include <QStringList>

class NetClass {
public:
  explicit NetClass();
  ~NetClass();

  const QString& name() const;
  void setName(const QString& name);

  QStringList& nets();
  const QStringList& nets() const;
  void setNets(const QStringList& nets);

  static int version();
  QDataStream& write(QDataStream& stream) const;
  QDataStream& read(QDataStream& stream);

private:
  QString m_name;
  QStringList m_nets;

  void readV0(QDataStream& stream);
};

Q_DECLARE_METATYPE(NetClass)

using NetClassList = QList<NetClass>;

inline QDataStream& operator<<(QDataStream& stream, const NetClass& netClass)
{
  return netClass.write(stream);
}

inline QDataStream& operator>>(QDataStream& stream, NetClass& netClass)
{
  return netClass.read(stream);
}