#!/bin/bash 

read N
SUM=0

for ((i=1;i<=$N;i++)) {
  read X
  ((SUM = $SUM + $X))
}

 ((AVG=$SUM/$N))
  echo $AVG
