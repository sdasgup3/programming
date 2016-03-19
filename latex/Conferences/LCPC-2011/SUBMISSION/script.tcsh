#! /bin/tcsh -vf

set file = $argv[1]
set head = $file:r

latex $head ;
if($status == 1) then
	exit(1)
endif

latex $head ;
if($status == 1) then
	exit(1)
endif

dvips $head.dvi ;
if($status == 1) then
	exit(1)
endif

ps2pdf $head.ps
if($status == 1) then
	exit(1)
endif

evince $head.pdf &
