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

#include "netclassmodel.h"
#include "netmodel.h"
#include "sessionmanager.h"
#include "uiabout.h"
#include "uimaterials.h"
#include "uinetproperties.h"
#include "uistackup.h"

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

UIMainWindow::UIMainWindow(QWidget* parent) :
  QMainWindow(parent),
  mp_ui(new Ui::UIMainWindow)
{
  mp_ui->setupUi(this);

  //Set up net item model
  NetModel* netModel = new NetModel(this);
  mp_ui->netsTable->setModel(netModel);

  NetClassModel* netClassModel = new NetClassModel(this);
  mp_ui->netClassesTable->setModel(netClassModel);

  connect(mp_ui->actionNew_Session, &QAction::triggered,
    this, &UIMainWindow::onNewSession);
  connect(mp_ui->actionOpen_Session, &QAction::triggered,
    this, &UIMainWindow::onOpenSession);
  connect(mp_ui->actionSave_Session, &QAction::triggered,
    this, &UIMainWindow::onSaveSession);
  connect(mp_ui->actionSave_Session_As, &QAction::triggered,
    this, &UIMainWindow::onSaveSessionAs);
  connect(mp_ui->actionExit, &QAction::triggered,
    this, &UIMainWindow::close);
  connect(mp_ui->actionAbout_Open_Stackup, &QAction::triggered,
    this, &UIMainWindow::onAbout);
  connect(mp_ui->actionStackup_Settings, &QAction::triggered,
    this, &UIMainWindow::onStackupSettings);
  connect(mp_ui->actionMaterials, &QAction::triggered,
    this, &UIMainWindow::onMaterials);
  connect(mp_ui->netsTable, &QTableView::clicked,
    this, &UIMainWindow::onNetClicked);
  connect(mp_ui->netClassesTable, &QTableView::clicked,
    this, &UIMainWindow::onNetClassClicked);

  connect(SessionManager::instance().get(), &SessionManager::sessionMarkedDirty,
    this, &UIMainWindow::onSessionMarkedDirty, Qt::QueuedConnection);
  connect(SessionManager::instance().get(), &SessionManager::sessionChanged,
    this, &UIMainWindow::updateWindowTitle, Qt::QueuedConnection);
}

UIMainWindow::~UIMainWindow()
{
  delete mp_ui;
}

void UIMainWindow::onNewSession()
{
  if (SessionManager::instance()->isSessionDirty()) {
    auto result = QMessageBox::question(this, "Save?",
      "Do you want to save the changes to this session before creating the new session?\nAny unsaved changes will be lost.",
      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

    if (result == QMessageBox::Cancel) {
      return;
    }
    else if (result == QMessageBox::Yes) {
        if (onSaveSession()) {
          return;
        }
      }
    }

    SessionManager::instance()->resetSession();
}

void UIMainWindow::onOpenSession()
{
  if (SessionManager::instance()->isSessionDirty()) {
    auto result = QMessageBox::question(this, "Save?",
      "Do you want to save the changes to this session before creating opening another session?\nAny unsaved changes will be lost.",
      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

    if (result == QMessageBox::Cancel) {
      return;
    }
    else {
      if (result == QMessageBox::Yes) {
        if (!onSaveSession()) {
          return;
        }
      }
    }
  }

  QString path = SessionManager::instance()->sessionFilename();
  if (path.isEmpty()) {
    path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  }

  QString fname = QFileDialog::getOpenFileName(this, "Open Session", path, "Open Stackup Session (*.oss)");

  if (!fname.isEmpty()) {
    if (!SessionManager::instance()->loadSession(fname)) {
      QMessageBox::critical(this, "Error", QString("Could not open session file at %1").arg(fname));
    }
  }
}

bool UIMainWindow::onSaveSession()
{
  bool result = false;

  QString fname;
  if (SessionManager::instance()->sessionFilename().isEmpty()) {
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    fname = QFileDialog::getSaveFileName(this, "Save Session", path, "Open Stackup Session (*.oss)");
  }
  else {
    fname = SessionManager::instance()->sessionFilename();
  }

  if (!fname.isEmpty()) {
    if (!SessionManager::instance()->saveSession(fname)) {
      QMessageBox::critical(this, "Error", QString("Could not write to %1\nChoose another location.").arg(fname));
    }
    else {
      result = true;
    }
  }

  return result;
}

void UIMainWindow::onSaveSessionAs()
{
  QString path = SessionManager::instance()->sessionFilename();
  if (path.isEmpty()) {
    path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  }

  QString fname = QFileDialog::getSaveFileName(this, "Save Session As", path, "Open Stackup Session (*.oss)");

  if (!fname.isEmpty()) {
    if (!SessionManager::instance()->saveSession(fname)) {
      QMessageBox::critical(this, "Error", QString("Could not write to %1\nChoose another location.").arg(fname));
    }
  }
}

void UIMainWindow::onSessionMarkedDirty(bool dirty)
{
  updateWindowTitle();
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

void UIMainWindow::closeEvent(QCloseEvent* event)
{
  if (SessionManager::instance()->isSessionDirty()) {
    auto result = QMessageBox::question(this, "Save?",
      "Do you want to save the changes to this session before closing?\nAny unsaved changes will be lost.",
      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes);

    if (result == QMessageBox::Yes) {
      //TODO: Save
    }
    else if (result == QMessageBox::Cancel) {
      event->ignore();
    }
  }
}

void UIMainWindow::onNetClicked(const QModelIndex& index)
{
  if (index.isValid()) {
    Net& net = SessionManager::instance()->nets()[index.row()];

    UINetProperties* propertiesUI = new UINetProperties(&net, this);
    showProperties(propertiesUI);
  }
}

void UIMainWindow::onNetClassClicked(const QModelIndex& index)
{
  if (index.isValid()) {
    NetClass& netClass = SessionManager::instance()->netClasses()[index.row()];
    showProperties(nullptr);
  }
}

void UIMainWindow::updateWindowTitle()
{
  QString title = "%1%2%3";

  if (SessionManager::instance()->isSessionDirty()) {
    title = title.arg("*");
  }
  else {
    title = title.arg("");
  }

  QString sessionFname = SessionManager::instance()->sessionFilename();
  if (!sessionFname.isEmpty()) {
    title = title.arg(QString("%1 - ").arg(sessionFname));
  }
  else {
    title = title.arg("");
  }

  title = title.arg(QApplication::applicationName());

  setWindowTitle(title);
}

void UIMainWindow::showProperties(QWidget* widget)
{
  QWidget* oldWidget = mp_ui->propertiesDock->widget();
  if (oldWidget) {
    oldWidget->deleteLater();
  }

  mp_ui->propertiesDock->setWidget(widget);
}
