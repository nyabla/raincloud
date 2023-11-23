module Requests exposing (..)

import Http
import Codec
import Config exposing (config)
import Url.Builder exposing (crossOrigin)

type alias AddTorrentResult = Result Http.Error Codec.TorrentAddResponse

addTorrent : String -> (AddTorrentResult -> msg) -> Cmd msg
addTorrent magnet message =
  Http.request
    { method = "POST"
    , headers = []
    , url = crossOrigin config.apiBase [ "addTorrent" ] []
    , body = Http.jsonBody
      <| Codec.torrentAddRequestEncoder
      <| Codec.TorrentAddRequest magnet
    , expect = Http.expectJson message Codec.torrentAddResponseDecoder
    , timeout = Just (30 * 1000)
    , tracker = Nothing
    }

type alias GetFilesListResult = Result Http.Error Codec.TorrentListFilesResponse

getTorrentFiles : String -> (GetFilesListResult -> msg) -> Cmd msg
getTorrentFiles infoHash message =
  Http.request
    { method = "GET" 
    , headers = []
    , url = crossOrigin config.apiBase [ "torrent", infoHash, "files" ] []
    , body = Http.emptyBody
    , expect = Http.expectJson message Codec.torrentListFilesResponseDecoder
    , timeout = Just (30 * 1000)
    , tracker = Nothing
    }
