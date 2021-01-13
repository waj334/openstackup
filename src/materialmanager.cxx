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

#include "materialmanager.h"

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

const Material MaterialManager::get(const QString& name) const
{
  Material material;

  auto it = m_materials.find(name);
  if (it != m_materials.end()) {
    material = it.value();
  }

  return material;
}

void MaterialManager::getAll(QList<Material>& materials) const
{
  materials = m_materials.values();
}

QString MaterialManager::typeString(const MaterialClass& type)
{
  QString typeStr = "Unknown";

  switch (type) {
  case MaterialClass::COPPER:
    typeStr = "Copper";
    break;
  case MaterialClass::CORE:
    typeStr = "Core";
    break;
  case MaterialClass::NONE:
    typeStr = "None";
    break;
  case MaterialClass::PREPREG:
    typeStr = "Prepreg";
    break;
  default:
    //Do nothing
    break;
  }

  return typeStr;
}