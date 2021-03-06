/*
 * This file is part of the Open Stackup distribution (https://github.com/waj334/openstackup).
 * Copyright (c) 2021 Justin A. Wilson.
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

#include <QObject>
#include <restinio/all.hpp>

class PluginServer : public QObject {
  Q_OBJECT
public:
  explicit PluginServer(QObject* parent = nullptr);
  ~PluginServer();

  bool start();

private:
  using router_t = restinio::router::express_router_t<>;
  using traits_t =
    restinio::traits_t<
    restinio::asio_timer_manager_t,
    restinio::shared_ostream_logger_t,
    router_t >;

  restinio::running_server_handle_t<traits_t> m_server;
  bool m_isRunning = false;

  std::unique_ptr<PluginServer::router_t> router();
};