#!/bin/sh

# Configure your favorite diff program here.
DIFF="/usr/bin/gvimdiff"

# Subversion provides the paths we need as the sixth and seventh
# parameters.
LEFT=${6}
RIGHT=${7}

# Call the diff command (change the following line to make sense for
# your merge program).
$DIFF $LEFT $RIGHT

# Return an errorcode of 0 if no differences were detected, 1 if some were.
# Any other errorcode will be treated as fatal.
# Add the following to $HOME/.subversion/config file
# [helpers]
# diff-cmd = /home/matt/bin/diffwrap.sh
