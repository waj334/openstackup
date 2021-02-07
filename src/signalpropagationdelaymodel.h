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

#include "net.h"

#include <QAbstractTableModel>

#include <QMap>

class SignalPropagationDelayModel : public QAbstractItemModel {
  Q_OBJECT
public:
  explicit SignalPropagationDelayModel(QObject* parent = nullptr);
  ~SignalPropagationDelayModel();

  // Inherited via QAbstractItemModel
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& child) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  
  enum Roles {
    NetRole = Qt::UserRole
  };

private slots:
  void onSync();

private:
  mutable QMap<size_t, QModelIndex> m_parentMap;

  double calculateDelay(int layer, double length, double traceWidth) const;
  double calculateStriplineDelay(double length, double traceWidth, const std::array<Layer,2> &layers) const;
  double calculateMicrostripDelay(double length, double traceWidth, const Layer& layer) const;

  double calculateEffectiveDk(double traceWidth, const Layer& layer) const;
  double calculateEffectiveDk(double traceWidth, const std::array<Layer, 2>& layers) const;
  double calculateDelay(double traceLength, double dk) const;
  double calculateMatchedLength(double t, double dk) const;

  struct TraceInfo {
    double dk = 0;
    double delay = 0;
  };

  QMap<size_t, TraceInfo> m_traceMap;
  QMap<QString, double> m_delayMap;
};