module Requests exposing (..)

import Http
import Codec
import Config exposing (config)
import Url.Builder exposing (crossOrigin)
import Json.Decode exposing (Decoder)

type RequestStatus a
  = None
  | Loading
  | Error String
  | Result a

type RequestError
  = Timeout
  | NetworkError
  | BadStatus Int String
  | BadBody String

type alias RequestResult response = Result RequestError response

type alias AddTorrentResult = RequestResult Codec.TorrentAddResponse

addTorrent : String -> (AddTorrentResult -> msg) -> Cmd msg
addTorrent magnet toMsg =
  Http.request
    { method = "POST"
    , headers = []
    , url = crossOrigin config.apiBase [ "addTorrent" ] []
    , body = Http.jsonBody
      <| Codec.torrentAddRequestEncoder
      <| Codec.TorrentAddRequest magnet
    , expect = expectApiResponse toMsg Codec.torrentAddResponseDecoder
    , timeout = Just (30 * 1000)
    , tracker = Nothing
    }

type alias GetFilesListResult = RequestResult Codec.TorrentListFilesResponse

getTorrentFiles : String -> (GetFilesListResult -> msg) -> Cmd msg
getTorrentFiles infoHash message =
  Http.request
    { method = "GET" 
    , headers = []
    , url = crossOrigin config.apiBase [ "torrent", infoHash, "files" ] []
    , body = Http.emptyBody
    , expect = expectApiResponse message Codec.torrentListFilesResponseDecoder
    , timeout = Just (30 * 1000)
    , tracker = Nothing
    }


-- EXPECT

expectApiResponse : (RequestResult a -> msg) -> Decoder a -> Http.Expect msg
expectApiResponse toMsg decoder =
  Http.expectStringResponse toMsg <|
    \response ->
      case response of
        Http.BadUrl_ _ ->
          -- pretending this doesn't exist sorry
          Err NetworkError

        Http.Timeout_ ->
          Err Timeout

        Http.NetworkError_ ->
          Err NetworkError

        Http.BadStatus_ metadata body ->
          Err (BadStatus metadata.statusCode body)

        Http.GoodStatus_ _ body ->
          case Json.Decode.decodeString decoder body of
            Ok value ->
              Ok value

            Err err ->
              Err (BadBody (Json.Decode.errorToString err))
