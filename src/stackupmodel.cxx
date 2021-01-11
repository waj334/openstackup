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

#include "stackupmodel.h"

constexpr int MAX_COLUMN = 4;

StackupModel::StackupModel(Layer* layers, size_t count, QObject* parent) :
  QAbstractItemModel(parent),
  mp_layers(layers),
  m_layerCount(count)
{

}

QModelIndex StackupModel::index(int row, int column, const QModelIndex& parent) const
{
  QModelIndex index;

  if (row >= 0 && row < m_layerCount && column >= 0 && column < MAX_COLUMN) {
    index = createIndex(row, column, &mp_layers[row]);
  }

  return index;
}

QModelIndex StackupModel::parent(const QModelIndex& child) const
{
  return QModelIndex();
}

int StackupModel::rowCount(const QModelIndex& parent) const
{
  if (parent.isValid()) {
    return 0;
  }

  return m_layerCount;
}

int StackupModel::columnCount(const QModelIndex& paren) const
{
  return MAX_COLUMN;
}

QVariant StackupModel::data(const QModelIndex& index, int role) const
{
  QVariant data;

  if (index.isValid()) {
    const Layer* layer = static_cast<Layer*>(index.internalPointer());

    switch (role) {
    case Qt::DisplayRole:
      switch (index.column()) {
      case 0:
        if (index.row() % 2 == 0) {
          data = (index.row()/2) + 1;
        }
        break;
      case 1:
        data = Material::typeString(layer->m_material.type());
        break;
      case 2:
        data = layer->m_material.name();
        break;
      case 3:
        data = QString("%1 %2")
          .arg(QString::number(layer->m_thickness, 'g', 2))
          .arg("mm"); //TODO: Handle units
      }
      break;
    case Qt::EditRole:
      switch (index.column()) {
      case 1:
        data = Material::typeString(layer->m_material.type());
        break;
      case 2:
        data = layer->m_material.name();
        break;
      case 3:
        data = layer->m_thickness;
      }
      break;
    }
  }

  return data;
}

QVariant StackupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant data;

  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      switch (section) {
      case 0:
        data = "Layer";
        break;
      case 1:
        data = "Type";
        break;
      case 2:
        data = "Material";
        break;
      case 3:
        data = "Thickness";
        break;
      }
    }
  }

  return data;
}
