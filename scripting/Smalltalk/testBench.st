m := Mandelbrot new.
m compute_n_print: 16 .
Transcript show: m.

Transcript open.
Transcript clear.

Transcript show: [ 1 timesRepeat: 
     [ 
        m compute_n_print: 16000.
        ]
] timeToRun.

MessageTally spyOn:
[ 1 timesRepeat: 
     [ 
        m compute_n_print: 16000.
        ]
].

