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

#include "netclass.h"

constexpr int NET_CLASS_VERSION = 0;

NetClass::NetClass()
{

}

NetClass::~NetClass()
{

}

const QString& NetClass::name() const
{
  return m_name;
}

void NetClass::setName(const QString& name)
{
  m_name = name;
}

QStringList& NetClass::nets()
{
  return m_nets;
}

const QStringList& NetClass::nets() const
{
  return m_nets;
}

void NetClass::setNets(const QStringList& nets)
{
  m_nets = nets;
}

int NetClass::version()
{
  return NET_CLASS_VERSION;
}

QDataStream& NetClass::write(QDataStream& stream) const {
  //Write version
  stream << NetClass::version();

  stream << m_name << m_nets;

  return stream;
}

QDataStream& NetClass::read(QDataStream& stream)
{
  //Read version
  int version = -1;
  stream >> version;

  switch (version) {
  case 0:
    readV0(stream);
    break;
  }

  return stream;
}

void NetClass::readV0(QDataStream& stream)
{
  stream >> m_name >> m_nets;
}