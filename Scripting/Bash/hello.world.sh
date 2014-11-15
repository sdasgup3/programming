#!/bin/bash -x
function printfiles {
  for file in $(ls); do
    echo $file
    let COUNTER=COUNTER+1
  done  
  echo "Number of Files $COUNTER"

  lsO=`ls`;
  for file in $lsO ; do
    echo print $file
  done
}

function quit {
  local VAR="Exiting"
  echo $VAR  
  exit
}

function decrementcounter {
  echo Argument $1
  local COUNTER=$1  
  while [  $COUNTER -ge 0 ]; do
  #while [  $COUNTER -gt 0 ]; do
    echo The counter is $COUNTER
    let COUNTER=COUNTER-1 
  done
}

#if [ -z "$1" ]; then
if [ $# -lt 1 ]; then
  echo usage: $0 -ls [-nols]
  exit
else
  echo Number of Arguments: $#
  echo Arguments are: $@
fi

#for opt in "$*";  do
for opt in "$@";  do
  echo Arg: $opt
done  

args=("$@")
for ((i=0; i < $# ; i++)) {
  echo "argument $((i+1)): ${args[$i]}"
}

COND=1
COUNTER=0
VAR="Printing files names"

if [ $COND = "1" ]; then
  echo $VAR
else
  echo "NONE"
fi  

printfiles
echo Global counter $COUNTER
decrementcounter $COUNTER
echo Global counter $COUNTER
quit


  
  
