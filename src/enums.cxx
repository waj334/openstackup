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

#include "enums.h"

QDataStream& operator<<(QDataStream& stream, const MaterialClass& mclass)
{
  QString str;

  switch (mclass) {
  case MaterialClass::COPPER:
    str = "COPPER";
    break;
  case MaterialClass::CORE:
    str = "CORE";
    break;
  case MaterialClass::NONE:
    str = "NONE";
    break;
  case MaterialClass::PREPREG:
    str = "PREPREG";
    break;
  }

  stream << str;
  return stream;
}

QDataStream& operator>>(QDataStream& stream, MaterialClass& mclass)
{
  QString value;
  stream >> value;

  if (value == "COPPER") {
    mclass = MaterialClass::COPPER;
  } 
  else if (value == "CORE") {
    mclass = MaterialClass::CORE;
  }
  else if (value == "NONE") {
    mclass = MaterialClass::NONE;
  }
  else if (value == "PREPREG") {
    mclass = MaterialClass::PREPREG;
  }

  return stream;
}