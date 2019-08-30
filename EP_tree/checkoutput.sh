#! /bin/bash

for name in `ls *.dataset`; do
  tmp1=`echo ${name#sched}`
  echo ${tmp1%.dataset}
  ls production*/${tmp1%.dataset}*.root | wc -l
  echo "the num of total list"
  ls list/sched${tmp1%.dataset}*.list | wc -l
done
