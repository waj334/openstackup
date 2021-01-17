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

#include "pluginserver.h"

PluginServer::PluginServer(QObject* parent) :
  QObject(parent)
{

}

PluginServer::~PluginServer()
{

}

bool PluginServer::start()
{
  m_isRunning = true;

  try {
    m_server = restinio::run_async<traits_t>(
      // Asio's io_context to be used.
      // HTTP-server will use own Asio's io_context object.
      restinio::own_io_context(),
      // The settings for the HTTP-server.
      restinio::server_settings_t<traits_t>{}
        .address("127.0.0.1")
        .port(8080)
        .request_handler(router()),
       // The size of thread-pool for the HTTP-server.
      16);
  }
  catch (std::exception e) {
    m_isRunning = false;
  }

  return true;
}

std::unique_ptr<PluginServer::router_t> PluginServer::router()
{
  auto router = std::make_unique< router_t >();

  router->http_post("/api/v1/nets/update",
    [](auto req, auto params) {
      auto resp = req->create_response();
      return resp.done();
    });

  return router;
}
