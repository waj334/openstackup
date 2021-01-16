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

#include "material.h"

#include <QDialog>
#include <QItemSelection>

class MaterialModel;
class Material;

namespace Ui {
  class UIMaterials;
}

class UIMaterials : public QDialog {
  Q_OBJECT
public:
  explicit UIMaterials(QWidget* parent = nullptr);
  ~UIMaterials();

protected slots:
  void onNewMaterial();
  void onDeleteMaterial();
  void onMaterialSelectionChanged(const QItemSelection&, const QItemSelection&);
  void onAddDk();
  void onDeleteDk();
  void onNameChanged(const QString& name);
  void onManufacturerChanged(const QString& manufacturer);
  void onTypeChanged(int index);

protected:
  void populateForm(Material* material);

private:
  Ui::UIMaterials* mp_ui;
  Material* mp_material;
  MaterialModel* mp_model;
};