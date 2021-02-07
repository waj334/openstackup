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

#include "layer.h"
#include "sessionmanager.h"
#include "signalpropagationdelaymodel.h"

#include <boost/functional/hash.hpp>

constexpr double C = 0.299792; //speed of light in mm/ps

SignalPropagationDelayModel::SignalPropagationDelayModel(QObject* parent) :
  QAbstractItemModel(parent)
{
  connect(SessionManager::instance().get(), &SessionManager::sync,
    this, &SignalPropagationDelayModel::onSync, Qt::QueuedConnection);
  connect(SessionManager::instance().get(), &SessionManager::sessionChanged,
    this, &SignalPropagationDelayModel::onSync, Qt::QueuedConnection);
}

SignalPropagationDelayModel::~SignalPropagationDelayModel()
{

}

QModelIndex SignalPropagationDelayModel::index(int row, int column, const QModelIndex& parent) const
{
  QModelIndex index;
  const auto& nets = SessionManager::instance()->nets();

  if (parent.isValid()) {
    const Net net = nets[parent.row()];
    QList<int> layerIds;
    net.layers(layerIds);

    if (row >= 0 && row < layerIds.size()) {
      size_t seed = parent.internalId();
      boost::hash_combine(seed, row);

      m_parentMap[seed] = parent;
      index = createIndex(row, column, seed);
    }
  }
  else if (row >= 0 && row < nets.size()) {
    size_t seed = -1;
    boost::hash_combine(seed, row);

    index = createIndex(row, column, seed);
  }

  return index;
}

QModelIndex SignalPropagationDelayModel::parent(const QModelIndex& child) const
{
  QModelIndex parent;
  const auto id = child.internalId();

  if (child.isValid() && m_parentMap.contains(id))
  {
    parent = m_parentMap[id];
  }

  return parent;
}

int SignalPropagationDelayModel::rowCount(const QModelIndex& parent) const
{
  int count = 0;
  const auto& nets = SessionManager::instance()->nets();
  const auto id = parent.internalId();

  if (parent.isValid())
  {
    //Note: Cheap hack to test for branch'ness
    size_t seed = -1;
    boost::hash_combine(seed, parent.row());

    // Is parent a branch node
    if (parent.internalId() == seed) {
      const Net net = nets[parent.row()];
      QList<int> layerIds;
      net.layers(layerIds);

      count = layerIds.size();
    }
  }
  else {
    count = nets.size();
  }

  return count;
}

int SignalPropagationDelayModel::columnCount(const QModelIndex& parent) const
{
  return 5;
}

QVariant SignalPropagationDelayModel::data(const QModelIndex& index, int role) const
{
  QVariant data;

  if (index.isValid()) {
    auto parent = this->parent(index);
    const auto& nets = SessionManager::instance()->nets();
    Net net;
    int layer = -1;

    if (parent.isValid()) {
      net = nets[parent.row()];
      
      QList<int> layerIds;
      net.layers(layerIds);

      layer = layerIds[index.row()];
    }
    else {
      net = nets[index.row()];
    }

    NetClass _class = SessionManager::instance()->netClass(net);

    size_t hash = boost::hash_value(net.name().toStdString());
    
    if (layer >= 0) {
      boost::hash_combine(hash, layer);
    }

    if (role == Qt::DisplayRole) {
      switch (index.column()) {
      case 0:
        //Only show name on parent nodes
        if (!parent.isValid()) {
          data = net.name();
        }
        break;
      case 1:
        if (layer >= 0) {
          data = QString::number(layer);
        }
        else {
          QString layerStr;

          QList<int> layerIds;
          net.layers(layerIds);

          if (!layerIds.isEmpty()) {
            layerStr = QString::number(layerIds.takeFirst());

            for (int layer : layerIds) {
              layerStr = QString("%1, %2")
                .arg(layerStr)
                .arg(layer);
            }

            data = layerStr;
          }
        }
        break;
      case 2:
        {
          double t = 0;
          if (layer >= 0) {
            t = m_traceMap[hash].delay;
          }
          else {
            QList<int> layerIds;
            net.layers(layerIds);

            //Total delay
            if (!layerIds.isEmpty()) {
              for (int layer : layerIds) {
                size_t layerHash = hash;
                boost::hash_combine(layerHash, layer);

                t += m_traceMap[layerHash].delay;
              }
            }
          }

          data = QString("%1 ps")
            .arg(t);
        }
        break;
      case 3:
        data = QString("%1 mm")
          .arg(net.length(layer), 0, 'f', 6);
        break;
      case 4:
        if (layer >= 0) {
          const double& maxDelay = m_delayMap[_class.name()];
          const double& dk = m_traceMap[hash].dk;

          const double dt = maxDelay - m_traceMap[hash].delay;
          const double dl = calculateMatchedLength(dt, dk);

          data = QString("%1 mm")
            .arg(net.length(layer) + dl, 0, 'f', 6);
        }
        break;
      default:
        //Do nothing
        break;
      }
    }
    else if (role == NetRole) {
      data = QVariant::fromValue<Net>(net);
    }
  }

  return data;
}

QVariant SignalPropagationDelayModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant data;

  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      switch (section) {
      case 0:
        data = "Net";
        break;
      case 1:
        data = "Layer";
        break;
      case 2:
        data = "Delay";
        break;
      case 3:
        data = "Length";
        break;
      case 4:
        data = "Delay Matched Length";
        break;
      default:
        //Do nothing
        break;
      }
    }
  }
  else if (orientation == Qt::Vertical) {

  }

  return data;
}

double SignalPropagationDelayModel::calculateDelay(int layer, double length, double traceWidth) const
{
  double t = 0;

  if (layer == 0) {
    // On the top layer, the dielectric material should
    // always be below the copper.
    const Layer& _layer = SessionManager::instance()->layers()[1];
    t = calculateMicrostripDelay(length, traceWidth, _layer);
  }
  else if (layer == 15) {
    // On the top layer, the dielectric material should
    // always be above the copper.
    const Layer& _layer = SessionManager::instance()->layers()[29];
    t = calculateMicrostripDelay(length, traceWidth, _layer);
  }
  else {
    // All other layers will have material above and below the copper
    std::array<Layer,2> layers;

    int copperLayer = layer * 2;

    layers[0] = SessionManager::instance()->layers()[copperLayer-1];
    layers[1] = SessionManager::instance()->layers()[copperLayer+1];

    t = calculateStriplineDelay(length, traceWidth, layers);
  }

  return t;
}

double SignalPropagationDelayModel::calculateStriplineDelay( double length, double traceWidth, const std::array<Layer,2>& layers) const
{
  double t = 0;

  const auto& layer0 = layers[0];
  const auto& layer1 = layers[1];

  const auto& dkList0 = layer0.material().permittivityList();
  const auto& dkList1 = layer1.material().permittivityList();

  const double h1 = layer0.thickness();
  const double h2 = layer1.thickness();

  if (!dkList0.empty() && !dkList1.empty()) {
    //TODO: Pick dk based on frequency of transmission
    const double er1 = dkList0[0].m_dk;
    const double er2 = dkList1[0].m_dk;

    double vp = 0;

    //Calculate effective dielectric constant
    double ee = (er1 * (h1 / (h1 + h2))) + (er2 * (h2 / (h1 + h2)));

    //Calculate propagation speed
    vp = C / sqrt(ee);

    //Calculate time to reach length
    t = length / vp;
  }

  return t;
}

double SignalPropagationDelayModel::calculateMicrostripDelay(double length, double traceWidth, const Layer& layer) const
{
  double t = 0;

  const auto& dkList = layer.material().permittivityList();

  if (!dkList.empty()) {
    //TODO: Pick dk based on frequency of transmission
    const double er = dkList[0].m_dk;

    double vp = 0;

    //Calculate effective dielectric constant
    double ee = ((er + 1) / 2) + (((er - 1) / 2) * (1.0 / sqrt(1 + (12 * (layer.thickness() / traceWidth)))));

    //Calculate propagation speed
    vp = C / sqrt(ee);

    //Calculate time to reach length
    t = length / vp;
  }

  return t;
}

double SignalPropagationDelayModel::calculateEffectiveDk(double traceWidth, const Layer& layer) const
{
  double dk = 0;

  const auto& dkList = layer.material().permittivityList();

  if (!dkList.empty()) {
    //TODO: Pick dk based on frequency of transmission
    const double er = dkList[0].m_dk;

    dk = ((er + 1) / 2) + (((er - 1) / 2) * (1.0 / sqrt(1 + (12 * (layer.thickness() / traceWidth)))));
  }

  return dk;
}

double SignalPropagationDelayModel::calculateEffectiveDk(double traceWidth, const std::array<Layer, 2>& layers) const
{
  double dk = 0;

  const auto& layer0 = layers[0];
  const auto& layer1 = layers[1];

  const auto& dkList0 = layer0.material().permittivityList();
  const auto& dkList1 = layer1.material().permittivityList();

  const double h1 = layer0.thickness();
  const double h2 = layer1.thickness();

  if (!dkList0.empty() && !dkList1.empty()) {
    //TODO: Pick dk based on frequency of transmission
    const double er1 = dkList0[0].m_dk;
    const double er2 = dkList1[0].m_dk;

    //Calculate effective dielectric constant
    dk = (er1 * (h1 / (h1 + h2))) + (er2 * (h2 / (h1 + h2)));
  }

  return dk;
}

double SignalPropagationDelayModel::calculateDelay(double traceLength, double dk) const
{
  const double vp = C / sqrt(dk);
  return traceLength / vp;
}

double SignalPropagationDelayModel::calculateMatchedLength(double t, double dk) const
{
  return (t * C) / sqrt(dk);
}

void SignalPropagationDelayModel::onSync()
{
  emit beginResetModel();
  m_parentMap.clear();
  m_traceMap.clear();
  m_delayMap.clear();

  //Precompute
  const NetList& nets = SessionManager::instance()->nets();

  for (const auto& net : nets) {
    //Create hash for net
    size_t netHash = boost::hash_value(net.name().toStdString());

    QList<int> layerIds;
    net.layers(layerIds);

    NetClass _class = SessionManager::instance()->netClass(net);
    if (!m_delayMap.keys().contains(_class.name())) {
      m_delayMap[_class.name()] = 0;
    }

    double& netClassMaxDelay = m_delayMap[_class.name()];
    double totalDelay = 0;

    for (const int& layerNumber : layerIds) {
      size_t layerHash = netHash;
      boost::hash_combine(layerHash, layerNumber);

      const double traceWidth = net.width(layerNumber);
      const double length = net.length(layerNumber);
      double dk = 0;

      if (layerNumber == 0) {
        // On the top layer, the dielectric material should
        // always be below the copper.
        const Layer& layer = SessionManager::instance()->layers()[1];
        dk = calculateEffectiveDk(traceWidth, layer);
      }
      else if (layerNumber == 15) {
        // On the top layer, the dielectric material should
        // always be above the copper.
        const Layer& layer = SessionManager::instance()->layers()[29];
        dk = calculateEffectiveDk(traceWidth, layer);
      }
      else {
        // All other layers will have material above and below the copper
        std::array<Layer, 2> layers;

        int copperLayer = layerNumber * 2;

        layers[0] = SessionManager::instance()->layers()[copperLayer - 1];
        layers[1] = SessionManager::instance()->layers()[copperLayer + 1];

        dk = calculateEffectiveDk(traceWidth, layers);
      }

      m_traceMap[layerHash].dk = dk;
      m_traceMap[layerHash].delay = calculateDelay(length, dk);

      //Add to delay for this net
      totalDelay += m_traceMap[layerHash].delay;
    }

    if (totalDelay > netClassMaxDelay) {
      netClassMaxDelay = totalDelay;
    }
  }

  emit endResetModel();
}