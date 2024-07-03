-- Autor: Petr Kaška (xkaska01)
-- Název projektu: FLP - Rozhodovací stromy 🌲
-- Datum: 26.2.2024
module ProcessInput
  ( argsParse,
    decisionBinaryTree,
  )
where

import DataStructures (Params (Params, file1, file2))
import Task1 (processMode1)
import Task2 (processMode2)

-- | Parses command line arguments into Params.
argsParse :: [String] -> Params
argsParse [] = error "Musíte zadat argumenty" -- Error if no arguments provided
argsParse [_] = error "Maly pocet argumentů" -- Error if too few arguments provided
argsParse (flag : firstFile : _) -- Pattern matching on flag and filenames
  | flag == "-2" = Params 2 firstFile "" -- If flag is "-2", set mode to 2 and provide file1
argsParse (flag : firstFile : secondFile : _) -- Pattern matching on flag and filenames
  | flag == "-1" = Params 1 firstFile secondFile -- If flag is "-1", set mode to 1 and provide both file1 and file2
argsParse _ = error "Neznámé parametry" -- Error for unknown parameters

-- | Processes the parameters and performs corresponding actions based on the mode.
decisionBinaryTree :: Params -> Int -> IO ()
decisionBinaryTree params mode
  | mode == 1 = processMode1 (file1 params) (file2 params) -- If mode is 1, invoke processMode1 with file1 and file2 from params
  | mode == 2 = processMode2 (file1 params) -- If mode is 2, invoke processMode2 with file1 from params
  | otherwise = error "Neznámý mode" -- If mode is neither 1 nor 2, throw an error for unknown mode
