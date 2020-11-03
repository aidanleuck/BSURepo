module Lib
    ( someFunc
     
      
    ) where
import System.Process
someFunc :: IO ()
someFunc = readProcess "wn" [] "" >>= putStrLn
