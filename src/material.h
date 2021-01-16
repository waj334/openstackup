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

#include <QList>
#include <QMetaType>
#include <QString>

class Material {
public:
  struct Permittivity {
    quint32 m_frequency;
    double m_dk;
  };

  using PermittivityList = QList<Permittivity>;

  Material();
  Material(QString name, MaterialClass mclass);
  ~Material();

  QString name() const;
  void setName(const QString& name);

  QString manufacturer() const;
  void setManufacturer(const QString& manufacturer);

  MaterialClass materialClass() const;
  void setMaterialClass(const MaterialClass& mclass);

  PermittivityList& permittivityList();
  const PermittivityList& permittivityList() const;

  double permittivity(int frequency);

  bool isValid() const;

  static int version();
  QDataStream& write(QDataStream& stream) const;
  QDataStream& read(QDataStream& stream);

private:
  QString m_name;
  QString m_manufacturer;

  MaterialClass m_class;
  PermittivityList m_dkList;

  void readV0(QDataStream& stream);
};

Q_DECLARE_METATYPE(Material)

inline QDataStream& operator<<(QDataStream& stream, const Material& material)
{
  return material.write(stream);
}

inline QDataStream& operator>>(QDataStream& stream, Material& material)
{
  return material.read(stream);
}