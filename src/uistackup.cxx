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

constexpr double MM_TO_PX = 150.0;

UIStackup::UIStackup(QWidget* parent) :
  QDialog(parent),
  mp_ui(new Ui::UIStackup),
  mp_scene(new QGraphicsScene)
{
  mp_ui->setupUi(this);
  mp_ui->graphicsView->setScene(mp_scene);

  //Create stack up model
  StackupModel* model = new StackupModel(m_layers.data(), m_layers.size(), this);
  mp_proxy = new StackupProxyModel(this);
  mp_proxy->setSourceModel(model);

  mp_ui->stackupTable->setModel(mp_proxy);
  mp_ui->stackupTable->setItemDelegate(new StackupDelegate(this));

  connect(mp_ui->layerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
    this, &UIStackup::onLayerCountChanged);
  connect(mp_ui->layerCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
    mp_proxy, &StackupProxyModel::setLayerCount);

  connect(model, &QAbstractItemModel::dataChanged, [this](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
    this->updateScene();
  });

  updateScene();
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
  updateScene();
}

void UIStackup::updateScene()
{
  //Clear scene
  mp_scene->clear();

  double y = 0;
  double totalThickness = 0;

  //Draw layers
  for (int i = 0; i < mp_proxy->rowCount(); ++i) {
    QModelIndex index = mp_proxy->mapToSource(mp_proxy->index(i, 2));
    Layer layer = m_layers[index.row()];
    QColor color;

    switch (layer.materialClass()) {
    case MaterialClass::COPPER:
      color = QColor("goldenrod");
      break;
    case MaterialClass::CORE:
      color = QColor("orangered");
      break;
    case MaterialClass::PREPREG:
      color = QColor("palegreen");
      break;
    }

    QRect viewport = mp_ui->graphicsView->viewport()->rect();

    double height = layer.thickness() * MM_TO_PX;
    QRectF rect(0, y, viewport.width()-1, height);
    y += height;

    mp_scene->addRect(rect, QPen(), QBrush(color));
    totalThickness += layer.thickness();
  }

  //Update total thickness
  QString str = QString("%1 mm")
    .arg(totalThickness, 0, 'f', 4);

  mp_ui->totalBoardThicknessLineEdit->setText(str);
}