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

#include <QMainWindow>

namespace Ui {
  class UIMainWindow;
}

class UIMainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit UIMainWindow(QWidget* parent = nullptr);
  ~UIMainWindow();

public slots:
  void onNewSession();
  void onOpenSession();
  bool onSaveSession();
  void onSaveSessionAs();

  void onSessionMarkedDirty(bool dirty);

  void onAbout();
  void onStackupSettings();
  void onMaterials();

  void onNetClicked(const QModelIndex& index);
  void onNetClassClicked(const QModelIndex& index);

protected:
  void closeEvent(QCloseEvent* event) override;

private slots:
  void updateWindowTitle();
  void showProperties(QWidget* widget);

private:
  Ui::UIMainWindow* mp_ui;
};