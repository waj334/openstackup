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

#include "layer.h"

#include <QDialog>
#include <QGraphicsScene>

#include <array>

class StackupProxyModel;

namespace Ui {
  class UIStackup;
}

class UIStackup : public QDialog {
  Q_OBJECT
public:
  explicit UIStackup(QWidget* parent = nullptr);
  ~UIStackup();

protected:
  void showEvent(QShowEvent*) override;

private slots:
  void onLayerCountChanged(int count);
  void updateScene() const;

private:
  Ui::UIStackup* mp_ui;

  StackupProxyModel* mp_proxy;
  QGraphicsScene* mp_scene;
};