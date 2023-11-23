/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "../dto/DTOs.hpp"
#include "../service/TorrentService.hpp"
#include "../util.hpp"

class TorrentController : public oatpp::web::server::api::ApiController {
public:
    typedef TorrentController __ControllerType;
    explicit TorrentController(const std::shared_ptr<ObjectMapper>& objectMapper)
        : oatpp::web::server::api::ApiController(objectMapper) {}; 

    static std::shared_ptr<TorrentController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
    ) {
        return std::make_shared<TorrentController>(objectMapper);
    }

    #include OATPP_CODEGEN_BEGIN(ApiController)

    ENDPOINT_ASYNC("POST", "/addTorrent", AddTorrent) {
        ENDPOINT_ASYNC_INIT(AddTorrent)

        Action act() override {
            return request->readBodyToDtoAsync<oatpp::Object<dto::TorrentAddRequest>>(controller->getDefaultObjectMapper())
                .callbackTo(&AddTorrent::addTorrentRequest);
        }

        Action addTorrentRequest(const oatpp::Object<dto::TorrentAddRequest>& request) {
            auto dto = request.get();
            auto magnet = dto->magnet.getValue("");
            return torrentService::AddTorrent::startForResult(magnet)
                .callbackTo(&AddTorrent::addTorrentResult);
        }

        Action addTorrentResult(const torrentService::AddTorrent::Result& result) {
            if (!result.ok()) {
                return _return(controller->createResponse(
                    Status::CODE_400,
                    result.error().message()
                ));
            }

            auto response = dto::TorrentAddResponse::createShared();
            response->infoHash = util::hashToHex(result.value());
            return _return(controller->createDtoResponse(
                Status::CODE_200,
                response
            ));
        }
    };

    ENDPOINT_ASYNC("GET", "/torrent/{infoHash}/files", ListFiles) {
        ENDPOINT_ASYNC_INIT(ListFiles)

        Action act() override {
            auto infoHashString = request->getPathVariable("infoHash").getValue("");

            lt::sha1_hash infoHash;

            try {
                const auto n = infoHash.size() * 8;
                infoHash = util::hexToHash<n>(infoHashString);
            } catch (const std::invalid_argument& error) {
                return _return(controller->createResponse(
                    Status::CODE_400,
                    error.what()
                ));
            }

            return torrentService::GetTorrentInfo::startForResult(infoHash)
                .callbackTo(&ListFiles::listFilesResponse);
        }

        Action listFilesResponse(const torrentService::GetTorrentInfo::Result& result) {
            if (!result.ok()) {
                return _return(controller->createResponse(
                    Status::CODE_404,
                    result.error().message()
                ));
            }

            auto torrentInfo = result.value();
            auto torrentFiles = torrentInfo->files();
            
            auto response = dto::TorrentListFilesResponse::createShared();
            response->files = {};

            for (auto fileIndex : torrentFiles.file_range()) {
                auto name = torrentFiles.file_name(fileIndex);
                response->files->push_back(name.to_string());
            }

            return _return(controller->createDtoResponse(
                Status::CODE_200,
                response
            ));
        }
    };

    #include OATPP_CODEGEN_END(ApiController)
};
