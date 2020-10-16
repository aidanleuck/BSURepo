module Main where

import Lib
import Control.Monad
import Control.Monad.IO.Class
import Data.IORef
import Graphics.UI.Gtk hiding (Action, backspace)

main :: IO ()
main = do
  void initGUI
  window <- windowNew
  widgetShowAll window
  mainGUIf

