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

#include "enums.h"
#include "material.h"

class Layer {
public:
  const Material material() const;
  void setMaterial(const Material& mat);

  double thickness() const;
  void setThickness(const double& thickness);

  MaterialClass materialClass() const;
  void setMaterialClass(const MaterialClass& mclass);

private:
  Material m_material;
  double m_thickness = 0;
  MaterialClass m_class = MaterialClass::NONE;
};