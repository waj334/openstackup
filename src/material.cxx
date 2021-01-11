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
  Material("", MaterialType::NONE, 0)
{
  //Does nothing
}

Material::Material(QString name, MaterialType type, double e) :
  m_name(name),
  m_type(type),
  m_dielectricCoeff(e)
{

}

Material::~Material()
{

}

QString Material::name() const
{
  return m_name;
}

Material::MaterialType Material::type() const
{
  return m_type;
}

double Material::dielectricCoefficient() const
{
  return m_dielectricCoeff;
}

bool Material::isValid() const
{
  return !m_name.isEmpty() && m_dielectricCoeff >= 1 
    && m_type != MaterialType::NONE;
}

QString Material::typeString(const MaterialType& type)
{
  QString typeStr = "Unknown";

  switch (type) {
  case MaterialType::COPPER:
    typeStr = "Copper";
    break;
  case MaterialType::CORE:
    typeStr = "Core";
    break;
  case MaterialType::NONE:
    typeStr = "None";
    break;
  case MaterialType::PREPREG:
    typeStr = "Prepreg";
    break;
  default:
    //Do nothing
    break;
  }

  return typeStr;
}
