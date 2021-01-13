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

#include <QMetaType>
#include <QString>

class Material {
public:
  Material();
  Material(QString name, MaterialClass mclass, double e);
  ~Material();

  QString name() const;
  void setName(const QString& name);

  MaterialClass materialClass() const;
  void setMaterialClass(const MaterialClass& mclass);

  double dielectricCoefficient() const;
  void setDielectricCoefficient(const double& e);

  bool isValid() const;

private:
  QString m_name;
  MaterialClass m_class;
  double m_dielectricCoeff = 0;
};

Q_DECLARE_METATYPE(Material)
