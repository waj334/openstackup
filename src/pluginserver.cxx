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

#include "net.h"
#include "pluginserver.h"
#include "sessionmanager.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

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
      //Parse JSON body
      QJsonParseError err;
      auto doc = QJsonDocument::fromJson(req->body().c_str(), &err);

      if (err.error == QJsonParseError::NoError) {
        auto root = doc.object()["nets"];
        if (root.isArray()) {
          NetList nets;
          NetClassList classes;

          //Convert array to list of nets
          for (const auto& element : root.toArray()) {
            auto obj = element.toObject();

            Net net(obj["name"].toString());
            NetClass netClass;
            
            //Handle net class
            QString netClassName = obj["class"].toString();
            auto it = std::find_if(classes.begin(), classes.end(), [&netClassName](const NetClass& netClass) {
              return netClass.name() == netClassName;
            });

            if (it == classes.end()) {
              //Add this net class
              NetClass netClass;
              netClass.setName(netClassName);
              netClass.nets() << net.name();

              classes << netClass;
            }
            else {
              (*it).nets() << net.name();
            }

            //Convert array of wire object
            for (const auto& element : obj["wires"].toArray()) {
              auto obj = element.toObject();
              Net::Wire wire;
              wire.m_layer = obj["layer"].toInt();
              wire.m_length = obj["length"].toDouble();
              
              auto& wires = net.wires();
              wires << wire;
            }

            nets << net;
          }

          //Update session
          SessionManager::instance()->setNets(nets);
          SessionManager::instance()->setNetClasses(classes);
        }
        else {
          return req->create_response(restinio::status_bad_request())
            .done();
        }
      }
      else {
        return req->create_response(restinio::status_bad_request())
          .done();
      }

      //Success!
      return req->create_response()
        .done();
    });

  return router;
}
