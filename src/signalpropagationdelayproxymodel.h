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

#include <QSortFilterProxyModel>

class SignalPropagationDelayProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
public:
  SignalPropagationDelayProxyModel(QObject* parent = nullptr);

public slots:
  void onNetClassChanged(const QString& name);

protected:
  bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

private:
  QString m_netClassName;
};