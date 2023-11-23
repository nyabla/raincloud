module Codec exposing (..)

import Json.Decode exposing (Decoder, field, string, list, map)
import Json.Encode

-- ADD TORRENT

type alias TorrentAddRequest =
  { magnetUri : String
  }

type alias TorrentAddResponse =
  { infoHash : String
  }

torrentAddRequestEncoder : TorrentAddRequest -> Json.Encode.Value
torrentAddRequestEncoder request =
  Json.Encode.object
    [ ( "magnet", Json.Encode.string request.magnetUri )
    ]

torrentAddResponseDecoder : Decoder TorrentAddResponse
torrentAddResponseDecoder =
  map TorrentAddResponse
    (field "infohash" string)

-- GET FILES

type alias TorrentListFilesResponse =
  { files : List String
  }

torrentListFilesResponseDecoder : Decoder TorrentListFilesResponse
torrentListFilesResponseDecoder =
  map TorrentListFilesResponse
    (field "files" (list string))
