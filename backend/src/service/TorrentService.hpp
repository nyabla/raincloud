/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "oatpp/core/async/Coroutine.hpp"

#include "oatpp/core/macro/component.hpp"

#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/torrent_flags.hpp>
#include <libtorrent/torrent_handle.hpp>

class TorrentService {
public:
    class AddTorrent : public oatpp::async::Coroutine<AddTorrent> {
    private:
        OATPP_COMPONENT(std::shared_ptr<lt::session>, m_session);

        const std::string m_magnet;
        lt::torrent_handle m_torrentHandle;
    public:
        AddTorrent(const std::string magnet) : m_magnet(magnet) {}

        Action act() override {
            auto torrentParams = lt::parse_magnet_uri(m_magnet);
            torrentParams.flags = lt::torrent_flags::default_dont_download;

            m_torrentHandle = m_session->add_torrent(torrentParams);

            return yieldTo(&AddTorrent::checkReady);
        }

        Action checkReady() {
            if (m_torrentHandle.torrent_file() == nullptr) {
                return waitRepeat(std::chrono::milliseconds(50));
            }

            return yieldTo(&AddTorrent::ready);
        }

        Action ready() {
            /* TODO */
            return finish();
        }
    };
};
