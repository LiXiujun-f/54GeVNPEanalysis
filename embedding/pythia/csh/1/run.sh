#! /bin/bash
cd $1/csh/$2
# sh runpythia.sh $3 $4 $1/out/$2
root simv2.C++\($3,$4,\"$1\/out\/$2\"\)
