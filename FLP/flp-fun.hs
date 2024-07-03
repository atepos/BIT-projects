-- Autor: Petr Kaška (xkaska01)
-- Název projektu: FLP - Rozhodovací stromy 🌲
-- Datum: 26.2.2024

import DataStructures (Params (..))
import ProcessInput (argsParse, decisionBinaryTree)
import System.Environment (getArgs)

-- Main body of the program
main :: IO ()
main = do
  args <- getArgs -- Retrieve command line arguments
  let params = argsParse args -- Parse the arguments into Params
  decisionBinaryTree params (mode params) -- Call the decisionBinaryTree function with the parsed parameters and their mode
