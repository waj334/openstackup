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
  Net("", -1, 0)
{

}

Net::Net(const QString& name, int layerNumber, double length) :
  m_name(name),
  m_layerNumber(layerNumber),
  m_length(length)
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
  return m_length;
}

void Net::setLength(double length)
{
  m_length = length;
}

int Net::layerNumber() const
{
  return m_layerNumber;
}

void Net::setLayerNumber(int layerNumber)
{
  m_layerNumber = layerNumber;
}