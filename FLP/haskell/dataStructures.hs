-- Autor: Petr Kaška (xkaska01)
-- Název projektu: FLP - Rozhodovací stromy 🌲
-- Datum: 26.2.2024
module DataStructures
  ( Tree (..),
    Threshold (..),
    Params (..),
    NewData,
    AllNewData,
    RecordForTraining (..),
    AllRecordsForTraining,
  )
where

-- | Data structure representing the parameters parsed from command line arguments.
data Params = Params
  { mode :: Int, -- Mode of operation -  1 or 2
    file1 :: String, -- First input file
    file2 :: String -- Second input file
  }
  deriving (Show)

-- | Data structure representing a threshold used in decision tree nodes.
data Threshold = Threshold Int Double
  deriving (Show, Eq)

-- | Data structure representing a decision tree.
data Tree
  = EmptyTree -- Empty tree
  | Node Threshold Tree Tree -- Decision node with threshold and left/right subtrees
  | Leaf String -- Leaf node with class name
  deriving (Show, Eq)

-- | Type alias for a list of new data points.
type NewData = [Double]

-- | Type alias for a list of lists of new data points (multiple sets of new data).
type AllNewData = [NewData]

-- | Data structure representing a record used for training decision trees.
data RecordForTraining = Record
  { values :: [Double], -- Feature values
    className :: String -- Class name
  }

-- | Type alias for a list of training records.
type AllRecordsForTraining = [RecordForTraining]
