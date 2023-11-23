module Main exposing (..)

import Browser
import Html exposing (..)
import Html.Attributes exposing (..)
import Browser exposing (Document)
import Html.Events exposing (onSubmit)
import Html.Events exposing (onInput)
import Requests exposing (addTorrent)

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

type InfoHashStatus
  = None
  | Loading
  | Error String
  | Hash String

type alias Model =
  { magnetUri : String
  , infoHash : InfoHashStatus
  }

init : () -> ( Model, Cmd Msg )
init _ =
  ( { magnetUri = "" 
    , infoHash = None
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
      ( { model | infoHash = Hash response.infoHash }
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

displayInfoHash : InfoHashStatus -> Html Msg
displayInfoHash infoHash =
  case infoHash of
    None ->
      text "no infohash"
    Loading ->
      text "loading infohash"
    Error message ->
      text ( "Error: " ++ message )
    Hash hash ->
      text hash

-- SUBSCRIPTIONS

subscriptions : Model -> Sub Msg
subscriptions _ =
    Sub.none
