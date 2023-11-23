module Config exposing (..)

type alias Config =
    { apiBase : String
    }

config : Config
config =
    { apiBase = "http://localhost:8080"
    }
