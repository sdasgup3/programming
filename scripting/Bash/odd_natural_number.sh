#!/bin/bash 

for ((i=1;i<=99;i=i+2)) {
  echo $i
}


for ((i=1;i<=99;i=i+1)) {
  cond=$((i%2))
  if [ $cond = 1 ]; then
    echo $i
  fi
}
