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

protected:
  void populateForm(Material* material);

private:
  Ui::UIMaterials* mp_ui;
  MaterialModel* mp_model;
};