#! /bin/bash
cd $1/csh/$2
# sh runpythia.sh $3 $4 $1/out/$2
source ~jiyj/.cshrc
root -l -b <<EOF
.L simv2.C++
simv2($3,$4,"$1/out/$2.gamma")
.q
EOF
