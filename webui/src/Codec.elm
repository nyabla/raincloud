module Codec exposing (..)

import Json.Decode exposing (Decoder, field, string, int, list, map, map2)
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
  { filesCount : Int
  , files : List String
  }

torrentListFilesResponseDecoder : Decoder TorrentListFilesResponse
torrentListFilesResponseDecoder =
  map2 TorrentListFilesResponse
    (field "filesCount" int)
    (field "files" (list string))
