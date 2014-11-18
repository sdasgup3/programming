#!/bin/bash -x 

read n
echo `echo "scale = 3; $n" | bc`
