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

double Net::length() const
{
  double l = 0;
  for (const auto& w : m_wires) {
    l += w.m_length;
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