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

#include "net.h"
#include "sessionmanager.h"

constexpr int NET_VERSION = 0;

Net::Net() :
  Net("")
{

}

Net::Net(const QString& name) :
  m_name(name)
{

}

Net::~Net()
{

}

QString Net::name() const
{
  return m_name;
}

void Net::setName(const QString& name)
{
  m_name = name;
}

double Net::length(int layer) const
{
  double l = 0;
  for (const auto& wire : m_wires) {
    if (layer == -1 || layer == wire.m_layer)
      l += wire.m_length;
  }

  return l;
}

Net::WireList& Net::wires()
{
  return m_wires;
}

const Net::WireList& Net::wires() const
{
  return m_wires;
}

void Net::updateWire(int index, const Wire& wire)
{
  if (index >= 0 && index < m_wires.count()) {
    m_wires[index] = wire;
  }
}

int Net::version()
{
  return NET_VERSION;
}

QDataStream& Net::write(QDataStream& stream) const {
  //Write version
  stream << Net::version();

  stream << m_name;

  stream << m_wires.count();
  for (const auto& wire : m_wires) {
    stream << wire.m_layer << wire.m_length;
  }

  return stream;
}

QDataStream& Net::read(QDataStream& stream)
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

void Net::layers(QList<int>& layers) const
{
  for (const auto& wire : m_wires) {
    //Check for duplicate
    if (!layers.contains(wire.m_layer))
    {
      layers << wire.m_layer;
    }
  }
}

void Net::readV0(QDataStream& stream)
{
  stream >> m_name;

  int count;
  stream >> count;

  for (int i = 0; i < count; ++i) {
    Wire wire;
    stream >> wire.m_layer >> wire.m_length;

    m_wires << wire;
  }
}