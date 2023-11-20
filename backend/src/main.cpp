/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "oatpp/network/Server.hpp"

#include "Components.hpp"
#include "controller/TorrentController.hpp"

void run() {
    // register components
    Components components;
    
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    router->addController(TorrentController::createShared());

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
    
    // create server
    oatpp::network::Server server(connectionProvider, connectionHandler);

    OATPP_LOGI("main", "server running on port %s", connectionProvider->getProperty("port").getData());

    // run server run!
    server.run();
}

int main() {
    // initialise environment
    oatpp::base::Environment::init();

    run();

    // tidy up environment
    oatpp::base::Environment::destroy();

    return 0;
}
