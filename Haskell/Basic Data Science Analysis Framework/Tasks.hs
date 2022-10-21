
-- =============== DO NOT MODIFY ===================

{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE FlexibleInstances #-}

-- ==================================================

module Tasks where

import Dataset
import Data.List
import Text.Printf
import Data.Array

import Common

type CSV = String
type Value = String
type Row = [Value]
type Table = [Row]
type ColumnName = String

-- Prerequisities
split_by :: Char -> String -> [String]
split_by x = foldr op [""]
  where op char acc
            | char == x = "":acc
            | otherwise = (char:head(acc)):tail(acc)

read_csv :: CSV -> Table
read_csv = (map (split_by ',')) . (split_by '\n')

write_csv :: Table -> CSV
write_csv = (foldr (++) []).
            (intersperse "\n").
            (map (foldr (++) [])).
            (map (intersperse ","))


{-
    TASK SET 1
-}


get_names :: Table -> [String]
get_names = map head

get_steps :: Table -> [[String]]
get_steps = map tail

op1 :: [String] -> Integer
op1 = foldr (\x y -> (read x :: Integer) + y) 0

op2 :: [Integer] -> [Float]
op2 [] = []
op2 (x:xs) = (fromIntegral x / 8):(op2 xs)

get_avg_steps :: Table -> [String]
get_avg_steps = (map (printf "%.2f")).op2.(map op1).get_steps

get_avg_steps_column :: Table -> [String]
get_avg_steps_column m = ("Average Number of Steps"):(tail (get_avg_steps m))

compute_average_steps :: Table -> Table
compute_average_steps m = (zipWith (:) (map head m)) (map (:[]) (get_avg_steps_column m))


-- Task 2

-- Number of people who have achieved their goal:
get_passed_people_num :: Table -> Int
get_passed_people_num m = foldr (\x y -> if (x >= 1000) then y + 1 else y) 0 (map op1 (get_steps m))


-- Percentage of people who have achieved their:

get_number_of_people :: Table -> Integer
get_number_of_people m = foldr (\x y -> y + 1) 0 (tail (map op1 (get_steps m)))

get_passed_people_percentage :: Table -> Float
get_passed_people_percentage m = (fromIntegral (get_passed_people_num m)) / (fromIntegral (get_number_of_people m))

-- Average number of daily steps

get_total_steps :: Table -> Integer
get_total_steps m = foldr (\x y -> y + x) 0 (tail (map op1 (get_steps m)))

get_steps_avg :: Table -> Float
get_steps_avg m = (fromIntegral (get_total_steps m)) / (fromIntegral (get_number_of_people m))


-- Task 3

get_total_steps_in_hour :: [String] -> Integer
get_total_steps_in_hour m = foldr (\x y -> (read x :: Integer) + y) 0 m

get_total_steps_by_hour :: Table -> [Integer]
get_total_steps_by_hour ([]:_) = []
get_total_steps_by_hour m = (get_total_steps_in_hour (map head m)) : (get_total_steps_by_hour (map tail m))

format_steps :: [Integer] -> Table -> [String]
format_steps [] m = []
format_steps (x:xs) m = (printf "%.2f" (((fromIntegral x) / (fromIntegral (get_number_of_people m))) :: Float)) : (format_steps xs m)

build_table :: [String] -> Table
build_table l = ["H10","H11","H12","H13","H14","H15","H16","H17"] : (l:[])

get_avg_steps_per_h :: Table -> Table
get_avg_steps_per_h m = build_table (format_steps (get_total_steps_by_hour (map tail (tail m))) m)


-- Task 4
get_minutes :: Table -> Table
get_minutes m = map tail (map tail (map tail m))

format_minutes_to_integer :: Table -> [[Integer]]
format_minutes_to_integer [[]] = [[]]
format_minutes_to_integer (x:[]) = (map (\a -> read a :: Integer) x) : []
format_minutes_to_integer (x:xs) = (map (\a -> read a :: Integer) x) : (format_minutes_to_integer xs)

count_minutes :: Integer -> Integer -> Integer -> [Integer] -> (Integer,Integer,Integer)
count_minutes a b c [] = (a,b,c)
count_minutes a b c (x:xs)
  | x < 50 = count_minutes (a+1) b c xs
  | (x < 100) && (x >= 50) = count_minutes a (b+1) c xs
  | x >= 100 = count_minutes a b (c+1) xs

get_very_active_minutes :: Table -> [Integer]
get_very_active_minutes m = map head (format_minutes_to_integer (tail (get_minutes m)))

get_fairly_active_minutes :: Table -> [Integer]
get_fairly_active_minutes m = map head (map tail (format_minutes_to_integer (tail (get_minutes m))))

get_lightly_active_minutes :: Table -> [Integer]
get_lightly_active_minutes m = map head (map tail (map tail (format_minutes_to_integer (tail (get_minutes m)))))

tuplet_to_row :: (Integer,Integer,Integer) -> String -> [String]
tuplet_to_row (a,b,c) s = s:((show a):((show b):((show c):[])))

get_activ_summary :: Table -> Table
get_activ_summary m = ["column","range1","range2","range3"] : ((tuplet_to_row (count_minutes 0 0 0 (get_very_active_minutes m)) "VeryActiveMinutes")
                      : ((tuplet_to_row (count_minutes 0 0 0 (get_fairly_active_minutes m)) "FairlyActiveMinutes")
                      : ((tuplet_to_row (count_minutes 0 0 0 (get_lightly_active_minutes m)) "LightlyActiveMinutes") : [])))


-- Task 5
get_total_steps_and_name :: Table -> [[String]]
get_total_steps_and_name m = zipWith (:) (map head m) (map (:[]) ((map head (map tail m)))) 

my_sorting_function :: [String] -> [String] -> Ordering
my_sorting_function [] [] = EQ 
my_sorting_function [] _ = LT 
my_sorting_function _ [] = GT
my_sorting_function (x:xs:[]) (y:ys:[]) = if ((compare (read xs :: Integer) (read ys :: Integer)) /= EQ) then (compare (read xs :: Integer) (read ys :: Integer)) else (compare x y)

get_ranking :: Table -> Table
get_ranking m = ["Name","Total Steps"]:(sortBy my_sorting_function (tail (get_total_steps_and_name m)))


-- Task 6
calc_average_4 :: [String] -> Float
calc_average_4 xs = fromIntegral (foldr (\x y -> y + (read x :: Integer)) 0 xs) / 4

get_average_4h_table :: Table -> [[Float]]
get_average_4h_table m = zipWith (:) (map calc_average_4 (map (take 4) (map tail m))) (map (:[]) (map calc_average_4 (map (take 4) (map reverse m))))

insert_difference_column :: [[Float]] -> [[Float]]
insert_difference_column ((x:y:[]):[]) = (x:y:(abs(x-y)):[]) : []
insert_difference_column ((x:y:[]):xs) = (x:y:(abs(x-y)):[]) : (insert_difference_column xs)

format_4h_to_table :: [[Float]] -> Table
format_4h_to_table [[]] = [[]]
format_4h_to_table (x:[]) = (map (printf "%.2f") x) : []
format_4h_to_table (x:xs) = (map (printf "%.2f") x) : (format_4h_to_table xs)

my_sorting_function_task6 :: [String] -> [String] -> Ordering
my_sorting_function_task6 [] [] = EQ 
my_sorting_function_task6 [] _ = LT 
my_sorting_function_task6 _ [] = GT
my_sorting_function_task6 (a:x:xs:xss:[]) (b:y:ys:yss:[]) = if ((compare (read xss :: Float) (read yss :: Float)) /= EQ) 
                                                      then (compare (read xss :: Float) (read yss :: Float)) else (compare a b)

get_steps_diff_table :: Table -> Table
get_steps_diff_table m = ["Name","Average first 4h","Average last 4h","Difference"] : 
                      (sortBy my_sorting_function_task6 (zipWith (:) (map head (tail m)) (format_4h_to_table (insert_difference_column (get_average_4h_table (tail m))))))


-- Task 7

-- Applies the given function to all the values
vmap :: (Value -> Value) -> Table -> Table
vmap f [[]] = [[]]
vmap f (x:[]) = (map f x) : []
vmap f m = (map f (head m)) : (vmap f (tail m))


-- Task 8

-- Applies the given function to all the entries
rmap :: (Row -> Row) -> [String] -> Table -> Table
rmap f s m =  s:(map f (tail m))

get_sleep_total :: Row -> Row
get_sleep_total r = (head r) : ((printf "%.2f" (foldr (\x y -> y + (read x :: Float)) 0 (tail r)) : []))


{-
    TASK SET 2
-}

-- Task 1
myComparing :: Int -> Row -> Row -> Ordering
myComparing i x y = if ((helpingComp i x y) == EQ) then (helpingComp 0 x y) else (helpingComp i x y)

findColumn :: ColumnName -> Row -> Int -> Int
findColumn s [] pos = 0
findColumn s (x:xs) pos = if (x == s) then pos else (findColumn s xs (pos + 1))

myIsDigit :: String -> Bool
myIsDigit c = ((head c) >= '0') && ((head c) <= '9')

helpingComp :: Int -> Row -> Row -> Ordering
helpingComp x a b = if ((a !! x) == "" || (b !! x) == "") then (compare (a !! x) (b !! x)) else (if (myIsDigit (a !! x)) then (compare (read (a !! x) :: Integer) (read (b !! x) :: Integer)) else (compare (a !! x) (b !! x)))  

tsort :: ColumnName -> Table -> Table
tsort column table = (head table) : (sortBy (myComparing (findColumn column (head table) 0)) (tail table))

-- Task 2
compareRows :: Row -> Row -> Ordering
compareRows [] [] = EQ
compareRows [] (y:ys) = LT
compareRows (x:xs) [] = GT
compareRows (x:xs) (y:ys) = if (x == y) then (compareRows xs ys) else (compare x y)  

vunion :: Table -> Table -> Table
vunion t1 t2 = if ((compareRows (head t1) (head t2)) == EQ) then (t1 ++ (tail t2)) else t1

-- Task 3
getTableLength :: Table -> Int
getTableLength t = length (map head t)

buildPaddingRow :: Int -> Row -> Row
buildPaddingRow 0 s = s
buildPaddingRow n s = buildPaddingRow (n-1) ("": s)

addPaddingToTable :: Table -> Int -> Table
addPaddingToTable t 0 = t
addPaddingToTable t n = addPaddingToTable (t ++ ((buildPaddingRow (length (head t)) []):[])) (n-1)

concatTableRows :: Table -> Table -> Table
concatTableRows t1 t2 = zipWith (++) t1 t2 

addPadding :: Table -> Table -> Table
addPadding t1 t2 = addPaddingToTable t1 ((getTableLength t2) - (getTableLength t1))

hunion :: Table -> Table -> Table
hunion t1 t2 = if ((getTableLength t1) > (getTableLength t2)) then (concatTableRows t1 (addPadding t2 t1))
                else (if ((getTableLength t1) < (getTableLength t2)) then (concatTableRows (addPadding t1 t2) t2)
                else (concatTableRows t1 t2))

-- Task 4

-- NOT DONE YET
tjoin :: ColumnName -> Table -> Table -> Table
tjoin key_column t1 t2 = t2

-- Task 5

cartesian :: (Row -> Row -> Row) -> [ColumnName] -> Table -> Table -> Table
cartesian new_row_function new_column_names t1 t2 = new_column_names:((foldr (\x y -> (foldr (\a b -> (new_row_function x a):b) [] (tail t2)) ++ y) [] (tail t1)))

-- Task 6

returnColumnByName :: String -> Table -> Table
returnColumnByName s [] = []
returnColumnByName s x = if ((head (head x)) == s) then (map (:[]) (map head x)) else (returnColumnByName s (map tail x)) 

projection :: [ColumnName] -> Table -> Table
projection [] t = foldr (\x y -> [] : y) [[]] t
projection columns_to_extract t = zipWith (++) (returnColumnByName (head columns_to_extract) t) (projection (tail columns_to_extract) t)

-- Task 7
verifyCondition :: (Value -> Bool) -> Table -> [Bool]
verifyCondition f [] = []
verifyCondition f ((x:xs):ys) = if (f x) then (True:(verifyCondition f ys)) else (False:(verifyCondition f ys))

buildFilteredTable :: [Bool] -> Table -> Table
buildFilteredTable [] _ = []
buildFilteredTable _ [] = []
buildFilteredTable (x:xs) (y:ys) = if (x) then (y:(buildFilteredTable xs ys)) else (buildFilteredTable xs ys)

filterTable :: (Value -> Bool) -> ColumnName -> Table -> Table
filterTable condition key_column t = (head t) : (buildFilteredTable (verifyCondition condition (tail (returnColumnByName key_column t))) (tail t))


{-
    TASK SET 3
-}


-- 3.1

data Query =
    FromTable Table
    | AsList String Query
    | Sort String Query
    | ValueMap (Value -> Value) Query
    | RowMap (Row -> Row) [String] Query
    | VUnion Query Query
    | HUnion Query Query
    | TableJoin String Query Query
    | Cartesian (Row -> Row -> Row) [String] Query Query
    | Projection [String] Query
    | forall a. FEval a => Filter (FilterCondition a) Query -- 3.4
    | Graph EdgeOp Query -- 3.5

instance Show QResult where
    show (List l) = show l
    show (Table t) = show t

class Eval a where
    eval :: a -> QResult

returnColumnByNameAsString :: String -> Table -> [String]
returnColumnByNameAsString s [] = []
returnColumnByNameAsString s x = if ((head (head x)) == s) then (map head x) else (returnColumnByNameAsString s (map tail x)) 

instance Eval Query where
    eval (FromTable t) = (Table t)
    eval (AsList colname (FromTable t)) =  (List (tail (returnColumnByNameAsString colname t)))
    eval (Sort colname (FromTable t)) = Table (tsort colname t)
    eval (ValueMap op (FromTable t)) = Table (vmap op t)
    eval (RowMap op colnames (FromTable t)) = Table (rmap op colnames t)
    eval (VUnion (FromTable t1) (FromTable t2)) = Table (vunion t1 t2)
    eval (HUnion (FromTable t1) (FromTable t2)) = Table (hunion t1 t2)
    eval (Cartesian op colnames (FromTable t1) (FromTable t2)) = Table (cartesian op colnames t1 t2)
    eval (TableJoin colname (FromTable t1) (FromTable t2)) = Table (tjoin colname t1 t2)
    eval (Projection colnames (FromTable t)) = Table (projection colnames t)
    eval (Filter a (FromTable t)) = Table ((head t):(filter (feval (head t) a) (tail t)))
    eval (Graph edgeOp (FromTable t)) = Table (["From","To","Value"] : (build_graph edgeOp (tail t) (tail t)))

-- 3.2 & 3.3

type FilterOp = Row -> Bool

data FilterCondition a =
    Eq String a |
    Lt String a |
    Gt String a |
    In String [a] |
    FNot (FilterCondition a) |
    FieldEq String String

class FEval a where
    feval :: [String] -> (FilterCondition a) -> FilterOp

getColumnValue :: [String] -> String -> Row -> Value
getColumnValue [] _ _ = ""
getColumnValue (x:xs) s (r:rs) = if (x == s) then r else (getColumnValue xs s rs)

checkIfInRow :: [String] -> String -> Bool
checkIfInRow [] _ = False
checkIfInRow (x:xs) s = if(x == s) then True else (checkIfInRow xs s)

compareColumnValues :: [String] -> String -> String -> Row -> Bool
compareColumnValues colnames colname1 colname2 row = (getColumnValue colnames colname1 row) == (getColumnValue colnames colname2 row)

getColumnValueFloat :: [String] -> String -> Row -> Float
getColumnValueFloat colnames colname row = read (getColumnValue colnames colname row) :: Float

instance FEval String where
    feval colnames (Eq colname a) = (== a).getColumnValue colnames colname
    feval colnames (Lt colname a) = (< a).getColumnValue colnames colname
    feval colnames (Gt colname a) = (> a).getColumnValue colnames colname   
    feval colnames (In colname list) = (checkIfInRow list).getColumnValue colnames colname
    feval colnames (FNot a) = not.feval colnames a
    feval colnames (FieldEq colname1 colname2) = compareColumnValues colnames colname1 colname2

instance FEval Float where
    feval colnames (Eq colname a) = (== a).getColumnValueFloat colnames colname
    feval colnames (Lt colname a) = (< a).getColumnValueFloat colnames colname
    feval colnames (Gt colname a) = (> a).getColumnValueFloat colnames colname   
    feval colnames (In colname list) = (checkIfInRow (map (show) list)).(show).getColumnValueFloat colnames colname
    feval colnames (FNot a) = not.feval colnames a
    feval colnames (FieldEq colname1 colname2) = compareColumnValues colnames colname1 colname2

-- 3.4

-- where EdgeOp is defined:
type EdgeOp = Row -> Row -> Maybe Value

show_maybe_just :: String -> String
show_maybe_just s = take ((length (tail(tail(tail(tail(tail(tail s))))))) - 1) (tail(tail(tail(tail(tail(tail s))))))

build_graph :: EdgeOp -> Table -> Table -> Table
build_graph edgeOp [] _ = []
build_graph edgeOp (x:xs) (t:ts) = (get_edges_for_node edgeOp x ts) ++ (build_graph edgeOp xs ts)

get_nodes_in_order :: String -> String -> Row
get_nodes_in_order s1 s2 = if (s1 > s2) then (s2:[s1]) else (s1:[s2])

get_edges_for_node :: EdgeOp -> Row -> Table -> Table
get_edges_for_node edgeOp _ [] = []
get_edges_for_node edgeOp row table = foldr (\x y -> if ((edgeOp row x) /= Nothing) then ((get_nodes_in_order (head x) (head row))++[(show_maybe_just (show (edgeOp row x)))]):y else y) [] table

-- 3.5
similarities_query :: Query
similarities_query = FromTable [["1", "2", "3"]]

-- 3.6 (Typos)
correct_table :: String -> Table -> Table -> Table
correct_table col csv1 csv2 = csv1
