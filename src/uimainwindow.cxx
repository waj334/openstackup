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

#include "uimainwindow.h"
#include "ui_uimainwindow.h"

#include "uiabout.h"
#include "uimaterials.h"
#include "uistackup.h"


UIMainWindow::UIMainWindow(QWidget* parent) :
  QMainWindow(parent),
  mp_ui(new Ui::UIMainWindow)
{
  mp_ui->setupUi(this);

  connect(mp_ui->actionAbout_Open_Stackup, &QAction::triggered,
    this, &UIMainWindow::onAbout);
  connect(mp_ui->actionStackup_Settings, &QAction::triggered,
    this, &UIMainWindow::onStackupSettings);
  connect(mp_ui->actionMaterials, &QAction::triggered,
    this, &UIMainWindow::onMaterials);
}

UIMainWindow::~UIMainWindow()
{
  delete mp_ui;
}

void UIMainWindow::onAbout()
{
  UIAbout* about = new UIAbout(this);
  about->setAttribute(Qt::WA_DeleteOnClose);
  about->exec();
}

void UIMainWindow::onStackupSettings()
{
  UIStackup* stackup = new UIStackup(this);
  stackup->setAttribute(Qt::WA_DeleteOnClose);
  stackup->exec();
}

void UIMainWindow::onMaterials()
{
  UIMaterials* materials = new UIMaterials(this);
  materials->setAttribute(Qt::WA_DeleteOnClose);
  materials->exec();
}
