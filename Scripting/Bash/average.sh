#!/bin/bash  -x

read N
SUM=0

for ((i=1;i<=$N;i++)) {
  read X
#SUM=`echo "scale = 3; $SUM + $X" | bc` 
    ((SUM = $SUM + $X))
}

echo `echo "scale = 3 ; $SUM / $N" | bc` 

