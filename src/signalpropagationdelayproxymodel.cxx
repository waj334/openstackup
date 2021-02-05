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

#include "net.h"
#include "sessionmanager.h"
#include "signalpropagationdelayproxymodel.h"
#include "signalpropagationdelaymodel.h"

SignalPropagationDelayProxyModel::SignalPropagationDelayProxyModel(QObject* parent) :
  QSortFilterProxyModel(parent)
{

}

void SignalPropagationDelayProxyModel::onNetClassChanged(const QString& name)
{
  m_netClassName = name;
  invalidateFilter();
}

bool SignalPropagationDelayProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
  bool accept = false;

  QModelIndex index;
  
  if (source_parent.isValid()) {
    index = source_parent.child(source_row, filterKeyColumn());
  }
  else {
    index = sourceModel()->index(source_row, filterKeyColumn());
  }

  if (index.isValid()) {
    const auto net = index.data(SignalPropagationDelayModel::NetRole).value<Net>();
    const auto _class = SessionManager::instance()->netClass(net);

    if (m_netClassName.isEmpty() || _class.name() == m_netClassName) {
      accept = true;
    }
  }
  
  accept &= QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
  return accept;
}
