/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <libtorrent/session.hpp>

#include "oatpp/core/macro/component.hpp"

class TorrentComponents {
    OATPP_CREATE_COMPONENT(std::shared_ptr<lt::session>, torrentSession)([] {
        return std::make_shared<lt::session>();
    }());
};
