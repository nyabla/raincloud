module Main exposing (..)

import Browser exposing (Document)
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (onSubmit, onInput)
import Requests exposing (addTorrent, RequestStatus(..))

-- MAIN

main : Program () Model Msg
main =
  Browser.document
    { init = init
    , view = view 
    , update = update
    , subscriptions = subscriptions
    }

-- MODEL

type alias Model =
  { magnetUri : String
  , infoHash : RequestStatus String
  , torrentFiles : RequestStatus (List String)
  }

init : () -> ( Model, Cmd Msg )
init _ =
  ( { magnetUri = "" 
    , infoHash = None
    , torrentFiles = None
    }
    , Cmd.none
  )

-- UPDATE

type Msg
  = MagnetChange String
  | MagnetSubmit
  | TorrentAdded Requests.AddTorrentResult
  | TorrentFiles Requests.GetFilesListResult

update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
  case msg of
    MagnetChange newUri ->
      ( { model | magnetUri = newUri }
      , Cmd.none
      )
    MagnetSubmit ->
      ( { model | infoHash = Loading }
      , addTorrent model.magnetUri TorrentAdded
      )
    TorrentAdded result ->
      gotInfoHash result model
    _ ->
      ( model, Cmd.none )

gotInfoHash : Requests.AddTorrentResult -> Model -> ( Model, Cmd Msg )
gotInfoHash result model =
  case result of
    Ok response ->
      ( { model | infoHash = Result response.infoHash }
      , Cmd.none
      )
    Err _ ->
      ( { model | infoHash = Error "oops" }
      , Cmd.none
      )

-- VIEW

view : Model -> Document Msg
view model =
  { title = "raincloud"
  , body =
    [ h1 [] [ text "raincloud" ]
    , Html.form [ onSubmit MagnetSubmit ]
      [ label [ for "magnet-input" ] [ text "magnet uri" ]
      , input [ id "magnet-input", type_ "text", onInput MagnetChange ] []
      , input [ type_ "submit" ] []
      ]
    , p [] [ displayInfoHash model.infoHash ]
    ]
  }

displayInfoHash : RequestStatus String -> Html Msg
displayInfoHash infoHash =
  case infoHash of
    None ->
      text "no infohash"
    Loading ->
      text "loading infohash"
    Error message ->
      text ( "Error: " ++ message )
    Result hash ->
      text hash

-- SUBSCRIPTIONS

subscriptions : Model -> Sub Msg
subscriptions _ =
    Sub.none
