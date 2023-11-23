module Config exposing (..)

type alias Config =
    { apiBase : String
    , requestTimeout : Maybe Float
    }

config : Config
config =
    { apiBase = "http://localhost:8080"
    , requestTimeout = Just (30 * 1000)
    }
