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

#include "material.h"

Material::Material() :
  Material("", MaterialClass::NONE)
{
  //Does nothing
}

Material::Material(QString name, MaterialClass mclass) :
  m_name(name),
  m_class(mclass)
{

}

Material::~Material()
{

}

QString Material::name() const
{
  return m_name;
}

void Material::setName(const QString& name)
{
  m_name = name;
}

QString Material::manufacturer() const
{
  return m_manufacturer;
}

void Material::setManufacturer(const QString& manufacturer)
{
  m_manufacturer = manufacturer;
}

MaterialClass Material::materialClass() const
{
  return m_class;
}

void Material::setMaterialClass(const MaterialClass& mclass)
{
  m_class = mclass;
}

Material::PermittivityList& Material::permittivityList()
{
  return m_dkList;
}

const Material::PermittivityList& Material::permittivityList() const
{
  return m_dkList;
}

double Material::permittivity(int frequency)
{
  double dk = 0;

  //Find dk value at frequency
  //TODO: Interpolate value???
  for (auto entry : m_dkList) {
    if (entry.m_frequency == frequency) {
      dk = entry.m_dk;
    }
  }

  return dk;
}

bool Material::isValid() const
{
  return !m_name.isEmpty() && !m_dkList.isEmpty() 
    && m_class != MaterialClass::NONE;
}
