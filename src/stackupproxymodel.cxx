/*
 * This file is part of the Open Stackup distribution (https://github.com/waj334/openstackup).
 * Copyright (c) 2021 Justin A. Wilson.
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

#include "stackupproxymodel.h"

StackupProxyModel::StackupProxyModel(QObject* parent) :
  QSortFilterProxyModel(parent)
{

}

void StackupProxyModel::setLayerCount(int count) {
  m_layerCount = count;
  invalidateFilter();
}

bool StackupProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
  bool keep = false;

  if (source_row < ((m_layerCount*2) / 2) ||
    source_row > sourceModel()->rowCount() - ((m_layerCount*2) / 2))
  {
    keep = true;
  }

  return keep;
}
