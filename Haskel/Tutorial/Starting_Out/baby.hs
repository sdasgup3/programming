doubleUs x y = doubleMe x + doubleMe y
doubleMe x = x + x

doubleSmallNumber x = if x > 100
                      then x
                      else doubleMe x

b = [[1,2,3,4],[5,3,3,3],[1,2,2,3,4],[1,2,3]]

--take 24 [13,16..]
c = [13,26..] 
x = [if x > 10 then "BOOM" else "BANG" | x <- [7..13], odd x]

boombang x  = [ if x > 10 then "BOOM"  else "BANG" | odd x ] -- accepting a number and return a list
boombang2 xs  = [ if x > 10 then "BOOM"  else "BANG" | x <- xs, odd x ] -- accepting a list and return a list

length' xs = sum [1 | _ <- xs]  -- interractive let length' xs = sum [1 | _ <- xs]

-- function that takes a string and removes everything except uppercase letters from it.
removeNonU xs = [ x | x <- xs , x `elem` ['A'..'Z']]

-- remove all odd numbers without flattening the list
let xxs = [[1,3,5,2,3,1,2,4,5],[1,2,3,4,5,6,7,8,9],[1,2,4,2,1,6,3,1,3,2,3,6]]
--  [[ x | x<- a , even x ]   | a <- xxs  ]

-- which right triangle that has integers for all sides and all sides equal to or smaller than 10 has a perimeter of 24? 
