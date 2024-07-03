-- Autor: Petr Kaška (xkaska01)
-- Název projektu: FLP - Rozhodovací stromy 🌲
-- Datum: 26.2.2024
-- Tento algoritmus vychází z velké části z tohoto zdroje: https://www.youtube.com/watch?v=_L39rN6gz7Y
module Task2
  ( processMode2,
  )
where

import Data.List (group, sort, sortBy)
import Data.List.Split (splitOn)
import DataStructures
  ( AllRecordsForTraining,
    RecordForTraining (..),
    Threshold (..),
    Tree (..),
  )
import System.IO (IOMode (ReadMode), hGetContents, withFile)

-- | Parses multiple lines of input data into a list of RecordForTraining.
allParseForPart2 :: String -> AllRecordsForTraining
allParseForPart2 = map parseForPart2 . lines
  where
    -- Parses a line of input data into a RecordForTraining.
    parseForPart2 :: [Char] -> RecordForTraining
    parseForPart2 line = Record value clasName
      where
        parts = splitOn "," line -- Split the line by commas
        clasName = last parts -- Extract class name (last element)
        value = map read $ init parts -- Extract feature values (all but last element), parsing them into Doubles

-- | Processes Mode 2: Train decision tree and print it.
processMode2 :: String -> IO ()
processMode2 file1 = do
  withFile file1 ReadMode $ \handle -> do
    contents <- hGetContents handle
    let records = allParseForPart2 contents
        decisionTree = trainDecisionTree records
    putStrLn $ printDecisionTree decisionTree

-- Funkce pro výpis stromu
-- printDecisionTree :: DecisionTree -> StringprintDecisionTree :: DecisionTree -> String
printDecisionTree :: Tree -> [Char]
printDecisionTree tree = printTree tree 0
  where
    printTree (Leaf cls) depth = replicate (depth * 2) ' ' ++ "Leaf: " ++ cls ++ "\n"
    printTree EmptyTree _ = ""
    printTree (Node (Threshold attrIndex threshold) left right) depth =
      replicate (depth * 2) ' '
        ++ "Node: "
        ++ show attrIndex
        ++ ", "
        ++ show threshold
        ++ "\n"
        ++ printTree left (depth + 1)
        ++ printTree right (depth + 1)

-- | Trains a decision tree based on the provided training data.
trainDecisionTree :: AllRecordsForTraining -> Tree
trainDecisionTree inputData
  -- If all training data belong to the same class, create a leaf node with that class.
  -- This represents a simple decision where all instances are assigned to the same class.
  | allSameClass inputData = Leaf $ className $ head inputData
  -- Otherwise, create a new internal node of the decision tree.
  | otherwise =
      Node threshold (trainDecisionTree left) (trainDecisionTree right)
  where
    -- Finding the threshold for data splitting.
    threshold = Threshold idx thr
    (idx, thr) = findTreshold inputData
    -- Splitting the data based on the found threshold.
    (left, right) = splitByFeatureThreshold threshold inputData

-- The function allSameClass determines whether all training data belong to the same class.
allSameClass :: AllRecordsForTraining -> Bool
allSameClass [] = True
allSameClass [_] = True
allSameClass (x : y : xs) = className x == className y && allSameClass (y : xs)

-- | Finds the optimal feature index and threshold for splitting the input data based on the lowest Gini impurity.
--   It takes a list of records 'input' and returns a tuple containing the feature index and threshold.
findTreshold :: AllRecordsForTraining -> (Int, Double) -- [Record { values = [1.0, 2.0, 3.0], className = "A" }, Record { values = [4.0, 5.0, 6.0], className = "B" }, ...]
findTreshold input =
  let -- Generate a list of feature indexes
      featureIndices = take (length $ values $ head input) [0 ..]

      -- Function to calculate the Gini impurity for a specific feature and find the optimal threshold
      calculateGiniForFeature featureIndex =
        let -- Extract the values of the specified feature from the input data and sort them
            featureVals = sort $ map ((!! featureIndex) . values) input -- featureIndex = 0, featureVals = [1.0, 4.0, ...]

            -- Calculates the average of each consecutive pair of values in the input list -> for example if input is [7,9] it returns [8]
            calculateMiddleValues (x : y : zs) = ((x + y) / 2) : calculateMiddleValues (y : zs)
            calculateMiddleValues _ = []
            middleVals = calculateMiddleValues featureVals

            -- Calculate the Gini impurity for each possible split threshold and pair it with the threshold value
            giniPairs = map (\x -> (giniImpurityByFeatureSplit (Threshold featureIndex x) input, x)) middleVals
            sortedPairs = sortBy (\(a, _) (b, _) -> compare a b) giniPairs -- Sort the results by Gini impurity in ascending order
         in (\(a, b) -> (featureIndex, a, b)) $ head sortedPairs -- Return the feature index and threshold with the lowest impurity

      -- Calculate the lowest Gini impurity for each feature
      giniResults = map calculateGiniForFeature featureIndices

      -- Sort the results by Gini impurity in ascending order
      sortedResults = sortBy (\(_, a, _) (_, b, _) -> compare a b) giniResults
   in (\(index, _, treshold) -> (index, treshold)) $ head sortedResults -- Return the feature index and threshold corresponding to the lowest impurity

-- | Calculates the Gini impurity for splitting a dataset based on a specified feature and threshold.
giniImpurityByFeatureSplit :: Threshold -> AllRecordsForTraining -> Double
giniImpurityByFeatureSplit (Threshold featureIdx splitThr) input =
  impurityLeft * (leftCount / totalCount) + impurityRight * (rightCount / totalCount) -- Weighted sum of impurities for the left and right subsets
  where
    (left, right) = splitByFeatureThreshold (Threshold featureIdx splitThr) input -- Split the input data into left and right subsets based on the specified feature index and threshold
    impurityLeft = gini $ map className left -- Calculate the Gini impurity for the left subset
    impurityRight = gini $ map className right -- Calculate the Gini impurity for the right subset
    totalCount = fromIntegral $ length input -- Total count of data points
    leftCount = fromIntegral $ length left -- Count of data points in the left subset
    rightCount = fromIntegral $ length right -- Count of data points in the right subset

-- | Splits the input dataset into two subsets based on a specified feature index and threshold.
-- The resulting subsets represent the partitioning of the dataset for binary splitting in decision tree induction.
splitByFeatureThreshold :: Threshold -> AllRecordsForTraining -> (AllRecordsForTraining, AllRecordsForTraining)
splitByFeatureThreshold (Threshold featureIdx splitThr) = foldr splitAccumulator ([], [])
  where
    splitAccumulator x (leftAcc, rightAcc)
      | (values x !! featureIdx) <= splitThr = (x : leftAcc, rightAcc)
      | otherwise = (leftAcc, x : rightAcc)

-- | Calculates the Gini impurity of a list of elements.
--   It takes a list of elements 'xs' and returns the Gini impurity.
gini :: [String] -> Double
gini classes =
  impurity
  where
    probabilities = calculateElementProbabilities classes -- Calculate the probabilities of each unique class
    impurity = 1 - foldl (\acc p -> p ** 2.0 + acc) 0 probabilities -- Calculate Gini impurity by subtracting sum of squared probabilities from 1

-- | Calculates the probabilities of each unique element in a list.
--   It takes a list of elements 'elementsList' and returns a list of probabilities corresponding to each unique element.
calculateElementProbabilities :: [String] -> [Double]
calculateElementProbabilities elementsList = map calculateProbability (group $ sort elementsList)
  where
    total = fromIntegral $ length elementsList -- Total number of elements in the list
    -- Function to calculate the probability of an element
    calculateProbability groupX = fromIntegral (length groupX) / total -- Probability of the current element
