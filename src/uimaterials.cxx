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
#include "permittivitydelegate.h"
#include "permittivitymodel.h"

UIMaterials::UIMaterials(QWidget* parent) :
  QDialog(parent),
  mp_ui(new Ui::UIMaterials)
{
  mp_ui->setupUi(this);
  mp_ui->editorArea->setDisabled(true);

  mp_model = new MaterialModel(this);
  mp_ui->materialsTable->setModel(mp_model);

  mp_ui->dkTable->setItemDelegate(new PermittivityDelegate(this));

  //Populate type drop down
  mp_ui->typeComboBox->addItem("", QVariant::fromValue<MaterialClass>(MaterialClass::NONE));
  mp_ui->typeComboBox->addItem("Core", QVariant::fromValue<MaterialClass>(MaterialClass::CORE));
  mp_ui->typeComboBox->addItem("Prepreg", QVariant::fromValue<MaterialClass>(MaterialClass::PREPREG));

  connect(mp_ui->newMaterialButton, &QPushButton::pressed,
    this, &UIMaterials::onNewMaterial);
  connect(mp_ui->deleteMaterialButton, &QPushButton::pressed,
    this, &UIMaterials::onDeleteMaterial);
  connect(mp_ui->materialsTable->selectionModel(), &QItemSelectionModel::selectionChanged,
    this, &UIMaterials::onMaterialSelectionChanged);
  connect(mp_ui->nameLineEdit, &QLineEdit::textEdited,
    this, &UIMaterials::onNameChanged);
  connect(mp_ui->manufacturerLineEdit, &QLineEdit::textEdited,
    this, &UIMaterials::onManufacturerChanged);
  connect(mp_ui->typeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    this, &UIMaterials::onTypeChanged);
  connect(mp_ui->addDkButton, &QPushButton::clicked, 
    this, &UIMaterials::onAddDk);
  connect(mp_ui->deleteDkButton, &QPushButton::clicked, 
    this, &UIMaterials::onDeleteDk);
}

UIMaterials::~UIMaterials()
{
  delete mp_ui;
}

void UIMaterials::onNewMaterial()
{
  int row = mp_model->rowCount();

  mp_model->insertRow(row);
  mp_ui->materialsTable->selectRow(row);
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
    mp_material = nullptr;
  }
  else {
    Material& material = MaterialManager::instance()->materials()[selection.first().row()];
    
    mp_ui->editorArea->setEnabled(true);
    mp_material = &material;
  }

  populateForm(mp_material);
}

void UIMaterials::onAddDk()
{
  auto model = mp_ui->dkTable->model();

  if (model) {
    int row = model->rowCount();
    model->insertRow(row);
  }
}

void UIMaterials::onDeleteDk()
{
  auto model = mp_ui->dkTable->model();

  if (model) {
    auto selection = mp_ui->dkTable->selectionModel()->selectedRows();

    for (const auto& index : selection) {
      model->removeRow(index.row());
    }
  }
}

void UIMaterials::onNameChanged(const QString& name)
{
  if (mp_material) {
    mp_material->setName(name);

    auto index = mp_ui->materialsTable->currentIndex();
    mp_model->dataChanged(index.siblingAtColumn(0), 
      index.siblingAtColumn(3));
  }
}

void UIMaterials::onManufacturerChanged(const QString& manufacturer)
{
  if (mp_material) {
    mp_material->setManufacturer(manufacturer);

    auto index = mp_ui->materialsTable->currentIndex()
      .siblingAtColumn(1);
    mp_model->dataChanged(index.siblingAtColumn(0), 
      index.siblingAtColumn(3));
  }
}

void UIMaterials::onTypeChanged(int index)
{
  if (index >= 0 && mp_material) {
    auto type = mp_ui->typeComboBox->itemData(index).value<MaterialClass>();
    mp_material->setMaterialClass(type);

    auto index = mp_ui->materialsTable->currentIndex();
    mp_model->dataChanged(index.siblingAtColumn(0), 
      index.siblingAtColumn(3));
  }
}

void UIMaterials::populateForm(Material* material)
{
  //Delete the old item model
  if (mp_ui->dkTable->model()) {
    mp_ui->dkTable->model()->deleteLater();
  }

  if (material) {
    mp_ui->nameLineEdit->setText(material->name());
    mp_ui->manufacturerLineEdit->setText(material->manufacturer());

    //Find item representing type
    int index = -1;
    for (int i = 0; i < mp_ui->typeComboBox->count(); ++i) {
      if (mp_ui->typeComboBox
        ->itemData(i).value<MaterialClass>() == material->materialClass()) {
        index = i;
        break;
      }
    }

    if (index >= 0) {
      mp_ui->typeComboBox->setCurrentIndex(index);
    }

    mp_ui->dkTable->setModel(new PermittivityModel(material));
  }
  else {
    mp_ui->nameLineEdit->clear();
    mp_ui->manufacturerLineEdit->clear();
  }
}
