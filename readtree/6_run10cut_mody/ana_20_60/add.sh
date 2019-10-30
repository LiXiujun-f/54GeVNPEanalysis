#! /bin/bash

#############################
#add all of the root files under the directory#
############################

START=`date +%s`
tmp="tmplist.txt"
out="incEv2.root"
N=`ls incEv2_*.root | wc -l`
rm -rf ${tmp}
ls incEv2_*.root > ${tmp}
if [ -f "${out}" ]; then
    echo "${out} already exit"
    exit 0
fi
#  root -l -q  myhadd.C\(`${out}`, `${tmp}`, `${N}`\) 

list=${out}" "
for i in `cat ${tmp}`; do
  list=${list}" "${i}
  # echo ${list}
done
echo ${list}
hadd ${list}

END=`date +%s`;
time=$((END-START))
echo $time

