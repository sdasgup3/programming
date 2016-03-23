#!/bin/sh
FILENAME=$1
convert -density 600 ${FILENAME}.pdf ${FILENAME}.png

convert ${FILENAME}-*.png  -append  ${FILENAME}.png

rm  ${FILENAME}-*.png
