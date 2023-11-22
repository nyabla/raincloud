/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {

class TorrentAddRequest : public oatpp::DTO {
    DTO_INIT(TorrentAddRequest, DTO);

    DTO_FIELD(String, magnet) = "";
};

class TorrentAddResponse : public oatpp::DTO {
    DTO_INIT(TorrentAddResponse, DTO);

    DTO_FIELD(String, infoHash, "infohash");
};

class TorrentListFilesRequest : public oatpp::DTO {
    DTO_INIT(TorrentListFilesRequest, DTO);

    DTO_FIELD(String, infoHash, "infohash") = std::string(40, '0');
};

class TorrentListFilesResponse : public oatpp::DTO {
    DTO_INIT(TorrentListFilesResponse, DTO);

    DTO_FIELD(Int16, filesCount);
    DTO_FIELD(Vector<String>, files);
};

}

#include OATPP_CODEGEN_END(DTO)
