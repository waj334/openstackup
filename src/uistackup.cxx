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

#include "uistackup.h"
#include "ui_uistackup.h"

#include "stackupdelegate.h"
#include "stackupmodel.h"
#include "stackupproxymodel.h"

UIStackup::UIStackup(QWidget* parent) :
  QDialog(parent),
  mp_ui(new Ui::UIStackup)
{
  mp_ui->setupUi(this);

  //Create stack up model
  StackupModel* model = new StackupModel(m_layers.data(), m_layers.size(), this);
  StackupProxyModel* proxy = new StackupProxyModel(this);
  proxy->setSourceModel(model);

  mp_ui->stackupTable->setModel(proxy);
  mp_ui->stackupTable->setItemDelegate(new StackupDelegate(this));

  connect(mp_ui->layerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
    this, &UIStackup::onLayerCountChanged);
  connect(mp_ui->layerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
    proxy, &StackupProxyModel::setLayerCount);
}

UIStackup::~UIStackup()
{
  delete mp_ui;
}

void UIStackup::onLayerCountChanged(int count)
{
  //Only allow even values
  int value = mp_ui->layerCountSpinBox->value();
  mp_ui->layerCountSpinBox->setValue(value + (value % 2));
}