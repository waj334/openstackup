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

#include "layer.h"

constexpr int LAYER_VERISON = 0;

const Material Layer::material() const {
  return m_material;
}

void Layer::setMaterial(const Material& mat) {
  m_material = mat;
}

double Layer::thickness() const {
  return m_thickness;
}
void Layer::setThickness(const double& thickness) {
  m_thickness = thickness;
}

MaterialClass Layer::materialClass() const {
  return m_class;
}

void Layer::setMaterialClass(const MaterialClass& mclass) {
  m_class = mclass;
}

int Layer::version()
{
  return LAYER_VERISON;
}

QDataStream& Layer::write(QDataStream& stream) const
{
  //Write version
  stream << version();

  stream << m_material << m_thickness << m_class;

  return stream;
}

QDataStream& Layer::read(QDataStream& stream)
{
  //Read version
  int version = -1;
  stream >> version;

  switch (version) {
  case 0:
    readV0(stream);
  }

  return stream;
}

void Layer::readV0(QDataStream& stream)
{
  stream >> m_material >> m_thickness >> m_class;
}
