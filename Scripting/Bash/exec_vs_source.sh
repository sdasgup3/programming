#!/bin/sh
# demonstrate setting a variable
echo "foo: "$(env | grep FOO)
export FOO=foo
echo "foo: "$(env | grep FOO)
# demonstrate changing of working directory
echo "PWD: "$PWD
cd ~/Documents
echo "PWD: "$PWD
