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

#include "uimaterials.h"
#include "ui_uimaterials.h"

#include "materialmanager.h"
#include "materialmodel.h"

UIMaterials::UIMaterials(QWidget* parent) :
  QDialog(parent),
  mp_ui(new Ui::UIMaterials)
{
  mp_ui->setupUi(this);
  mp_ui->editorArea->setDisabled(true);

  mp_model = new MaterialModel(this);
  mp_ui->materialsTable->setModel(mp_model);

  connect(mp_ui->newMaterialButton, &QPushButton::pressed,
    this, &UIMaterials::onNewMaterial);
  connect(mp_ui->deleteMaterialButton, &QPushButton::pressed,
    this, &UIMaterials::onDeleteMaterial);
  connect(mp_ui->materialsTable->selectionModel(), &QItemSelectionModel::selectionChanged,
    this, &UIMaterials::onMaterialSelectionChanged);
}

UIMaterials::~UIMaterials()
{
  delete mp_ui;
}

void UIMaterials::onNewMaterial()
{
  auto index = mp_ui->materialsTable->currentIndex();
  int row = mp_model->rowCount();

  if (index.isValid()) {
    row = index.row();
  }

  mp_model->insertRow(row);
}

void UIMaterials::onDeleteMaterial()
{
  auto selection = mp_ui->materialsTable->selectionModel()->selectedRows();

  for (auto index : selection) {
    mp_model->removeRow(index.row());
  }
}

void UIMaterials::onMaterialSelectionChanged(const QItemSelection&, const QItemSelection&)
{
  auto selection = mp_ui->materialsTable->selectionModel()->selectedRows();

  if (selection.isEmpty()) {
    mp_ui->editorArea->setDisabled(true);
    populateForm(nullptr);
  }
  else {
    Material& material = MaterialManager::instance()->materials()[selection.first().row()];
    
    mp_ui->editorArea->setEnabled(true);
    populateForm(&material);
  }
}

void UIMaterials::populateForm(Material* material)
{
  mp_ui->tableWidget->clearContents();

  if (material) {
    mp_ui->nameLineEdit->setText(material->name());
    mp_ui->manufacturerLineEdit->setText(material->manufacturer());

    Material::PermittivityList& list = material->permittivityList();
    mp_ui->tableWidget->setRowCount(list.count());

    int row = 0;

    //Populate the permittivity table
    for (auto entry : list) {
      QString freqStr = QString("%1 MHz")
        .arg(entry.m_frequency);

      QTableWidgetItem* freqItem = new QTableWidgetItem(freqStr);
      QTableWidgetItem* dkItem = new QTableWidgetItem(QString::number(entry.m_dk, 'f'));

      mp_ui->tableWidget->setItem(row, 0, freqItem);
      mp_ui->tableWidget->setItem(row, 1, dkItem);
      ++row;
    }
  }
  else {
    mp_ui->nameLineEdit->clear();
    mp_ui->manufacturerLineEdit->clear();
  }
}
