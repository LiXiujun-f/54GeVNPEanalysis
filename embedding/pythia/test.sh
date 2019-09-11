#! /bin/bash
root -l <<EOF
.L simv2.C++
simv2($1,$1,"out/$3.gamma")
.q
EOF  
