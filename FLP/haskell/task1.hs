-- Autor: Petr Kaška (xkaska01)
-- Název projektu: FLP - Rozhodovací stromy 🌲
-- Datum: 26.2.2024
module Task1
  ( processMode1,
  )
where

import Data.List.Split (splitOn)
import DataStructures
  ( AllNewData,
    NewData,
    Threshold (..),
    Tree (..),
  )
import System.IO (IOMode (ReadMode), hGetContents, withFile)

-- | Parses a string representing multiple lines into a list of lists of 'Double'.
allParseForPart1 :: String -> AllNewData
allParseForPart1 allLines = map parseForPart1 $ lines allLines
  where
    -- Parses a string into a list of elements of type 'b'.
    parseForPart1 :: (Read b) => [Char] -> [b]
    parseForPart1 line = map read $ splitOn "," line

-- | Processes Mode 1: Classify data based on decision tree.
processMode1 :: String -> String -> IO ()
processMode1 file1 file2 = do
  withFile file1 ReadMode $ \handle1 -> do
    -- Open file1 in read mode
    contents <- hGetContents handle1 -- Read contents of file1
    let decisionTree = insertDecisionTree $ lines contents -- Parse contents into decision tree
    withFile file2 ReadMode $ \handle2 -> do
      -- Open file2 in read mode
      newDataContents <- hGetContents handle2 -- Read contents of file2
      let classifications = classifWholeDataInput decisionTree $ allParseForPart1 newDataContents -- Classify new data using decision tree
      putStr $ unlines (init classifications) -- Print classifications except the last one
      putStr (last classifications) -- Print the last classification

-- | Function to create a leaf node.
createLeaf :: String -> Tree
createLeaf = Leaf

-- | Function to create a node.
createNode :: Threshold -> Tree -> Tree -> Tree
createNode = Node

-- | Parses nodes from list of strings and creates a decision tree.
insertDecisionTree :: [String] -> Tree
insertDecisionTree = fst . parseNodes
  where
    -- Parses nodes from list of strings.
    parseNodes :: [String] -> (Tree, [String])
    parseNodes [] = (EmptyTree, []) -- If input list is empty, return an empty tree and an empty list
    parseNodes (line : rest) =
      case words (map (\c -> if c == ',' then ' ' else c) line) of -- Split the line by spaces, replacing commas with spaces
        "Node:" : idxStr : threshStr : _ ->
          -- If the line starts with "Node:", parse the index and threshold
          let idx = read idxStr :: Int -- Parse index as Int
              thr = read threshStr :: Double -- Parse threshold as Double
              (leftSubtree, remaining1) = parseNodes rest -- Recursively parse left subtree
              (rightSubtree, remaining2) = parseNodes remaining1 -- Recursively parse right subtree
           in (createNode (Threshold idx thr) leftSubtree rightSubtree, remaining2) -- Create node with parsed index and threshold, and remaining list
        "Leaf:" : className : _ -> (createLeaf className, rest) -- If the line starts with "Leaf:", create a leaf node with class name and return it with remaining list
        _ -> error "Vstup ve špatném formátu" -- If the line doesn't match any expected format, throw an error

-- | Classifies entire input data.
classifWholeDataInput :: Tree -> AllNewData -> [String]
classifWholeDataInput _ [] = [] -- If input data is empty, return empty list
classifWholeDataInput tree (newData : newDataTail) =
  -- Classify each set of new data
  classifOne tree newData : classifWholeDataInput tree newDataTail -- Classify one set of data and recursively classify the rest

-- | Classifies one set of data.
classifOne :: Tree -> NewData -> String
classifOne = clasi -- Alias for clasi function
  where
    clasi :: Tree -> NewData -> String
    clasi EmptyTree _ = error "Prázdný strom" -- If tree is empty, throw error
    clasi (Leaf className) _ = className -- If node is leaf, return class name
    clasi (Node (Threshold idx trh) leftTree rightTree) inputNewData =
      -- If node is decision node
      if inputNewData !! idx > trh -- Check if input data value at index exceeds threshold
        then clasi rightTree inputNewData -- Recursion on right subtree
        else clasi leftTree inputNewData -- Recursion on left subtree
